#include "cimmerian.h"

#define FOV 60
#define WALL_HEIGHT 35456
/*
    Logic for WALL_HEIGHT is `MAP_CELL_LEN * dist_proj_plane`.
    Second value is `man.tex[man.curr_tex]->width / (2.0f * f_tan(DEG2RAD(FOV)/2))`.
    In other words: `WALL_HEIGHT = 64 * 554`.
*/

static void draw_floor_and_ceiling(void);
static int is_player_out_of_bounds(const VectorF pos, const Map* m);
static void raycasting(const Map* m);
static double get_horizontal_distance(const Map* m, const int map_val, 
    const double tan, const double ray_angle);
static double get_vertical_distance(const Map* m, const int map_val, 
    const double tan, const double ray_angle);
static void fix_fisheye_effect(double* distance, const double ray_angle);
static void draw_wall(const Color color, const double distance, const int ray);

void draw_game(void)
{
    draw_floor_and_ceiling();
    if (!is_player_out_of_bounds(man.player.pos, man.map))
        raycasting(man.map);
    draw_minimap(man.map);
    return;
}

void reset_global_coordinates(void)
{
    set_minimap_display(0);
    reset_player_transform(man.map);
    return;
}

static void draw_floor_and_ceiling(void)
{
    Vertex v;
    Vector size;
    Texture *t;

    t = man.tex[man.curr_tex];

    v.coords.x = 0;
    v.coords.y = 0;
    v.color = get_color_rgba(109, 101, 98, 255);
    size.x = t->width;
    size.y = t->height * 0.5f;
    draw_rectangle_full(t, v, size);

    v.coords.y = t->height*0.5f;
    v.color = get_color_rgba(59, 92, 169, 255);
    draw_rectangle_full(t, v, size);
    return;
}

static int is_player_out_of_bounds(const VectorF pos, const Map* m)
{
    if (pos.x < 0 || pos.x > m->width * MAP_CELL_LEN)
        return 1;
    else if (pos.y < 0 || pos.y > m->height * MAP_CELL_LEN)
        return 1;
    return 0;
}

static void raycasting(const Map* m)
{
    Texture *t = man.tex[man.curr_tex];
    const double ray_increment = RAD_1/(t->width/FOV);
    const int max_distance = MAP_CELL_LEN * MAX_CELL_AMOUNT;
    /* `ray_angle = man.player.angle` for center */
    double ray_angle = clamp_rad(man.player.angle + FOV/2.0f * RAD_1);
    double tan, horizontal, vertical, distance;
    int ray;
    Color color;

    ray = t->width;
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

static double get_horizontal_distance(const Map* m, const int map_val, 
    const double tan, const double ray_angle)
{
    const double atan = -1/tan;
    double distance = 1000000;
    int depth_of_field = 0;
    int map_index;
    VectorF hit, offset;
    Vector max;

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
        max.y = m->height - (int)(hit.y/MAP_CELL_LEN);
        if (ray_angle < RAD_180) --max.y;
        map_index = max.y*m->width+max.x;
        if (map_index >= 0 && map_index < m->width*m->height 
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

static double get_vertical_distance(const Map* m, const int map_val, 
    const double tan, const double ray_angle)
{
    const double ntan = -tan;
    double distance = 1000000;
    int depth_of_field = 0;
    int map_index;
    VectorF hit, offset;
    Vector max;

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
        max.y = m->height-1 - (int)(hit.y/MAP_CELL_LEN);
        if (ray_angle > RAD_90 && ray_angle < RAD_270) --max.x;
        map_index = max.y*m->width+max.x;
        if (map_index >= 0 && map_index < m->width*m->height 
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

static void fix_fisheye_effect(double* distance, const double ray_angle)
{
    *distance *= f_cos(clamp_rad(man.player.angle - ray_angle));
    return;
}

static void draw_wall(const Color color, const double distance, const int ray)
{
    Texture* t = man.tex[man.curr_tex];
    const double height = WALL_HEIGHT / distance;
    Vertex v1, v2;
    v1.coords.x = t->width - ray;
    v1.coords.y = (t->height - height) / 2;
    v1.color = color;
    v2.coords.x = v1.coords.x;
    v2.coords.y = v1.coords.y + (t->height + height) / 2;
    v2.color = color;
    draw_line(t, v1, v2);
    return;
}
