#include "cimmerian.h"

static void draw_gradient(t_tex* t);

void draw_game(void)
{
    draw_gradient(man.tex[man.curr_tex]);
    return;
}

static void draw_gradient(t_tex* t)
{
    t_vert v;

    v.color.b = 255 / 2;
    v.color.a = 255;
    v.coord.y = 0;
    while (v.coord.y < t->size.y)
    {
        v.coord.x = 0;
        while (v.coord.x < t->size.x)
        {
            v.color.r = 255 * v.coord.y / t->size.y;
            v.color.g = 255 * v.coord.x / t->size.x;
            draw_point(t, v.color, v.coord.x, v.coord.y);
            ++v.coord.x;
        }
        ++v.coord.y;
    }
    return;
}

/* -------------------------------------------------------------------------- */

#define FOV 60
#define WALL_HEIGHT 35456
/*
    Logic for WALL_HEIGHT is `MAP_CELL_LEN * dist_proj_plane`.
    Second value is `man.tex[man.curr_tex]->size.x / (2.0f * f_tan(DEG2RAD(FOV)/2))`.
    In other words: `WALL_HEIGHT = 64 * 554`.
*/

static int is_player_out_of_bounds(t_vec2 pos, t_map* m);
static void raycasting(t_map* m);
static double get_horizontal_distance(t_map* m, int map_val, double tan, double ray_angle);
static double get_vertical_distance(t_map* m, int map_val, double tan, double ray_angle);
static void fix_fisheye_effect(double* distance, double ray_angle);
static void draw_wall(t_color color, double distance, int ray);

void reset_global_coordinates(void)
{
    set_minimap_display(0);
    reset_player_transform(man.map);
    return;
}

static int is_player_out_of_bounds(t_vec2 pos, t_map* m)
{
    if (pos.x < 0 || pos.x > m->size.x * MAP_CELL_LEN)
        return 1;
    else if (pos.y < 0 || pos.y > m->size.y * MAP_CELL_LEN)
        return 1;
    return 0;
}

static void raycasting(t_map* m)
{
    t_tex* t;
    double ray_increment;
    int max_distance;
    double ray_angle;
    double tan;
    double horizontal;
    double vertical;
    double distance;
    int ray;
    t_color color;

    t = man.tex[man.curr_tex];
    ray_increment = RAD_1/(t->size.x/FOV);
    max_distance = MAP_CELL_LEN * MAX_CELL_AMOUNT;
    /* `ray_angle = man.player.angle` for center */
    ray_angle = clamp_rad(man.player.angle + FOV/2.0f * RAD_1);
    ray = t->size.x;
    while (ray > 0)
    {
        tan = f_tan(ray_angle);
        horizontal = get_horizontal_distance(m, 1, tan, ray_angle);
        vertical = get_vertical_distance(m, 1, tan, ray_angle);

        if (vertical <= horizontal)
        {
            distance = vertical;
            color = get_color_rgba(109, 101, 189, 255);
        }
        else
        {
            distance = horizontal;
            color = get_color_rgba(128, 101, 164, 255);
        }

        if (distance > max_distance)
        {
            distance = max_distance;
            color = get_color_rgba(0, 0, 0, 255);
        }
        fix_fisheye_effect(&distance, ray_angle);
        draw_wall(color, distance, ray);

        ray_angle = clamp_rad(ray_angle - ray_increment);

        --ray;
    }
    return;
}

static double get_horizontal_distance(t_map* m, int map_val, double tan, double ray_angle)
{
    double atan;
    double distance;
    int depth_of_field;
    int map_index;
    t_vec2 hit;
    t_vec2 offset;
    t_ivec2 max;

    atan = -1/tan;
    distance = 1000000;
    depth_of_field = 0;
    /* If ray is looking down */
    if (ray_angle > RAD_180)
    {
        hit.y = (int)(man.player.pos.y/MAP_CELL_LEN) * MAP_CELL_LEN;
        hit.x = (man.player.pos.y - hit.y) * atan + man.player.pos.x;
        offset.y = -MAP_CELL_LEN;
        offset.x = -offset.y*atan;
    }
    /* If ray is looking up */
    else if (ray_angle < RAD_180)
    {
        hit.y = (int)((man.player.pos.y+MAP_CELL_LEN)/MAP_CELL_LEN) * MAP_CELL_LEN;
        hit.x = (man.player.pos.y - hit.y) * atan + man.player.pos.x;
        offset.y = MAP_CELL_LEN;
        offset.x = -offset.y*atan;
    }
    else
    {
        depth_of_field = MAX_CELL_AMOUNT;
    }

    /* From ray to map array index */
    while (depth_of_field < MAX_CELL_AMOUNT)
    {
        max.x = (int)(hit.x/MAP_CELL_LEN);
        max.y = m->size.y - (int)(hit.y/MAP_CELL_LEN);
        if (ray_angle < RAD_180) --max.y;
        map_index = max.y*m->size.x+max.x;
        if (map_index >= 0 && map_index < m->size.x*m->size.y 
            && m->data[map_index] == map_val)
        {
            distance = get_distance(man.player.pos, hit);
            depth_of_field = MAX_CELL_AMOUNT;
        }
        else
        {
            hit.x += offset.x;
            hit.y += offset.y;
            ++depth_of_field;
        }
    }
    return distance;
}

static double get_vertical_distance(t_map* m, int map_val, double tan, double ray_angle)
{
    double ntan;
    double distance;
    int depth_of_field;
    int map_index;
    t_vec2 hit;
    t_vec2 offset;
    t_ivec2 max;

    ntan = -tan;
    distance = 1000000;
    depth_of_field = 0;
    /* If ray is looking left */
    if (ray_angle > RAD_90 && ray_angle < RAD_270)
    {
        hit.x = (int)(man.player.pos.x/MAP_CELL_LEN) * MAP_CELL_LEN;
        hit.y = (man.player.pos.x - hit.x) * ntan + man.player.pos.y;
        offset.x = -MAP_CELL_LEN;
        offset.y = -offset.x*ntan;
    }
    /* If ray is looking right */
    else if (ray_angle < RAD_90 || ray_angle > RAD_270)
    {
        hit.x = (int)((man.player.pos.x+MAP_CELL_LEN)/MAP_CELL_LEN) * MAP_CELL_LEN;
        hit.y = (man.player.pos.x - hit.x) * ntan + man.player.pos.y;
        offset.x = MAP_CELL_LEN;
        offset.y = -offset.x*ntan;
    }
    else
    {
        depth_of_field = MAX_CELL_AMOUNT;
    }

    /* From ray to map array index */
    while (depth_of_field < MAX_CELL_AMOUNT)
    {
        max.x = (int)(hit.x/MAP_CELL_LEN);
        max.y = m->size.y-1 - (int)(hit.y/MAP_CELL_LEN);
        if (ray_angle > RAD_90 && ray_angle < RAD_270) --max.x;
        map_index = max.y*m->size.x+max.x;
        if (map_index >= 0 && map_index < m->size.x*m->size.y 
            && m->data[map_index] == map_val)
        {
            distance = get_distance(man.player.pos, hit);
            depth_of_field = MAX_CELL_AMOUNT;
        }
        else
        {
            hit.x += offset.x;
            hit.y += offset.y;
            ++depth_of_field;
        }
    }
    return distance;
}

static void fix_fisheye_effect(double* distance, double ray_angle)
{
    *distance *= f_cos(clamp_rad(man.player.angle - ray_angle));
    return;
}

static void draw_wall(t_color color, double distance, int ray)
{
    t_tex* t;
    double height;
    t_vert v1;
    t_vert v2;

    t = man.tex[man.curr_tex];
    height = WALL_HEIGHT / distance;
    v1.coord.x = t->size.x - ray;
    v1.coord.y = (t->size.y - height) / 2;
    v1.color = color;
    v2.coord.x = v1.coord.x;
    v2.coord.y = v1.coord.y + (t->size.y + height) / 2;
    v2.color = color;
    draw_line(t, v1, v2);
    return;
}
