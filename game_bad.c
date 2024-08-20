#include "cimmerian.h"

static void draw_gradient(t_tex* t);
static void raycasting(void);
static double get_dist_h(t_vec2 ray_dir);
static double get_dist_v(t_vec2 ray_dir);
static double get_dist(double dist_h, double dist_v, t_color* color);
static void draw_wall(t_tex* t, int ray_index, double dist, t_color color);

static void draw_map(void);
static void draw_player(void);

void draw_game(void)
{
    draw_gradient(man.tex[man.curr_tex]);
    raycasting();
    /*
    draw_map();
    draw_player();
    */
    return;
}

/*
    Horizontal gradient from a dark blue/purple color at the top to a dark 
    greyish brown at the bottom
*/
static void draw_gradient(t_tex* t)
{
    t_vert v;
    t_color top;
    t_color bottom;
    float factor;

    top = get_color_rgba(64, 0, 64, 255);
    bottom = get_color_rgba(58, 38, 42, 255);
    v.coord.y = 0;
    while (v.coord.y < t->size.y)
    {
        v.coord.x = 0;
        while (v.coord.x < t->size.x)
        {
            factor = (float)v.coord.y / (t->size.y - 1);
            v.color.r = top.r + factor * (bottom.r - top.r);
            v.color.g = top.g + factor * (bottom.g - top.g);
            v.color.b = top.b + factor * (bottom.b - top.b);
            v.color.a = 255;
            draw_point(t, v.color, v.coord.x, v.coord.y);
            ++v.coord.x;
        }
        ++v.coord.y;
    }
}

/* TODO: Temporary */
static void draw_map(void)
{
    t_ivec2 i;
    t_ivec2 size;
    t_vert vert;

    i.y = 0;
    while (i.y < man.map->size.y)
    {
        i.x = 0;
        while (i.x < man.map->size.x)
        {
            if (man.map->data[i.y * man.map->size.x + i.x] == 1)
                vert.color = get_color_rgba(0xFF, 0xFF, 0xFF, 0xFF);
            else
                vert.color = get_color_rgba(0x00, 0x00, 0x00, 0xFF);
            size.x = MAP_CELL_LEN;
            size.y = MAP_CELL_LEN;
            vert.coord.x = i.x * size.x;
            vert.coord.y = i.y * size.y;
            --size.x;
            --size.y;
            draw_rectangle_full(man.tex[man.curr_tex], vert, size);
            ++i.x;
        }
        ++i.y;
    }
    return;
}

/* TODO: Temporary */
static void draw_player(void)
{
    t_ivec2 size;
    t_vert vert1;
    t_vert vert2;

    vert1.color = get_color_rgba(0x00, 0xFF, 0xFF, 0xFF);
    vert1.coord.x = man.player.pos.x;
    vert1.coord.y = man.player.pos.y;
    vert2.color = get_color_rgba(0xFF, 0x00, 0x00, 0xFF);
    vert2.coord.x = vert1.coord.x + man.player.delta.x * 5;
    vert2.coord.y = vert1.coord.y + man.player.delta.y * 5;
    draw_line(man.tex[man.curr_tex], vert1, vert2);
    size.x = 8;
    size.y = 8;
    vert1.coord.x -= size.x / 2;
    vert1.coord.y -= size.y / 2;
    draw_rectangle_full(man.tex[man.curr_tex], vert1, size);
    vert1.color = get_color_rgba(0x00, 0x00, 0x00, 0xFF);
    draw_rectangle(man.tex[man.curr_tex], vert1, size);
    return;
}

#define FOV_SCALE 0.66
static void raycasting(void)
{
    int screen_width;
    double camera_x;
    int ray_index;
    double dist;
    t_color color;
    t_vec2 ray_dir;
    t_vec2 plane;

    screen_width = man.tex[man.curr_tex]->size.x;

    /* Calculate the camera plane vector (perpendicular to the direction vector) */
    plane.x = -man.player.delta.y * (FOV_SCALE);
    plane.y = man.player.delta.x * (FOV_SCALE);

    ray_index = 0;
    while (ray_index < screen_width)
    {
        /* Calculate the camera x-coordinate (-1 to 1) for the current ray */
        camera_x = 2 * ray_index / (double)screen_width - 1;

        /* Calculate the ray direction vector */
        ray_dir.x = man.player.delta.x + plane.x * camera_x;
        ray_dir.y = man.player.delta.y + plane.y * camera_x;

        /* Get the distance to the nearest wall */
        dist = get_dist(get_dist_h(ray_dir), get_dist_v(ray_dir), &color);

        /* Draw the wall slice */
        if (dist > 0)
            draw_wall(man.tex[man.curr_tex], ray_index, dist, color);

        ++ray_index;
    }
    return;
}

static double get_dist(double dist_h, double dist_v, t_color* color)
{
    double dist;

    /* Get shortest dist */
    if (dist_v < 0 || (dist_h > 0 && dist_h < dist_v))
    {
        dist = dist_h;
        *color = get_color_rgba(74, 42, 77, 255);
    }
    else
    {
        dist = dist_v;
        *color = get_color_rgba(84, 43, 88, 255);
    }
    return dist;
}

static double get_dist_h(t_vec2 ray_dir)
{
    double dist_h;
    int depth_of_field;
    t_vec2 h;
    t_vec2 ray;
    t_vec2 offset;
    t_ivec2 map;

    dist_h = -1;
    depth_of_field = 0;
    h.x = man.player.pos.x;
    h.y = man.player.pos.y;

    /* Looking up */
    if (ray_dir.y < 0)
    {
        /* Round to the nearest 64th value */
        ray.y = (((int)man.player.pos.y >> 6) << 6) - 0.0001;
        ray.x = man.player.pos.x + (man.player.pos.y - ray.y) / ray_dir.y * ray_dir.x;
        offset.y = -MAP_CELL_LEN;
        offset.x = offset.y / ray_dir.y * ray_dir.x;
    }
    /* Looking down */
    else if (ray_dir.y > 0)
    {
        /* Round to the nearest 64th value */
        ray.y = (((int)man.player.pos.y >> 6) << 6) + MAP_CELL_LEN;
        ray.x = man.player.pos.x + (man.player.pos.y - ray.y) / ray_dir.y * ray_dir.x;
        offset.y = MAP_CELL_LEN;
        offset.x = offset.y / ray_dir.y * ray_dir.x;
    }
    /* Looking straight left or right */
    else
    {
        ray.x = man.player.pos.x;
        ray.y = man.player.pos.y;
        depth_of_field = 8;
    }

    while (depth_of_field < 8)
    {
        map.x = (int)(ray.x) >> 6;
        map.y = (int)(ray.y) >> 6;
        if (map.x < 0 || map.y < 0 || map.x >= man.map->size.x || map.y >= man.map->size.y)
            break;
        if (man.map->data[map.y * man.map->size.x + map.x] == 1)
        {
            h.x = ray.x;
            h.y = ray.y;
            dist_h = get_distance(man.player.pos, h);
            depth_of_field = 8;
            if (dist_h / MAP_CELL_LEN > depth_of_field)
                dist_h = -1;
            break;
        }
        else
        {
            ray.x += offset.x;
            ray.y += offset.y;
            depth_of_field += 1;
        }
    }
    return dist_h;
}

static double get_dist_v(t_vec2 ray_dir)
{
    double dist_v;
    int depth_of_field;
    t_vec2 v;
    t_vec2 ray;
    t_vec2 offset;
    t_ivec2 map;

    dist_v = -1;
    depth_of_field = 0;
    v.x = man.player.pos.x;
    v.y = man.player.pos.y;

    /* Looking left */
    if (ray_dir.x < 0)
    {
        /* Round to the nearest 64th value */
        ray.x = (((int)man.player.pos.x >> 6) << 6) - 0.0001;
        ray.y = man.player.pos.y + (man.player.pos.x - ray.x) / ray_dir.x * ray_dir.y;
        offset.x = -MAP_CELL_LEN;
        offset.y = offset.x / ray_dir.x * ray_dir.y;
    }
    /* Looking right */
    else if (ray_dir.x > 0)
    {
        /* Round to the nearest 64th value */
        ray.x = (((int)man.player.pos.x >> 6) << 6) + MAP_CELL_LEN;
        ray.y = man.player.pos.y + (man.player.pos.x - ray.x) / ray_dir.x * ray_dir.y;
        offset.x = MAP_CELL_LEN;
        offset.y = offset.x / ray_dir.x * ray_dir.y;
    }
    /* Looking straight up or down */
    else
    {
        ray.x = man.player.pos.x;
        ray.y = man.player.pos.y;
        depth_of_field = 8;
    }

    while (depth_of_field < 8)
    {
        map.x = (int)(ray.x) >> 6;
        map.y = (int)(ray.y) >> 6;
        if (map.x < 0 || map.y < 0 || map.x >= man.map->size.x || map.y >= man.map->size.y)
            break;
        if (man.map->data[map.y * man.map->size.x + map.x] == 1)
        {
            v.x = ray.x;
            v.y = ray.y;
            dist_v = get_distance(man.player.pos, v);
            depth_of_field = 8;
            if (dist_v / MAP_CELL_LEN > depth_of_field)
                dist_v = -1;
        }
        else
        {
            ray.x += offset.x;
            ray.y += offset.y;
            depth_of_field += 1;
        }
    }
    return dist_v;
}

static void draw_wall(t_tex* t, int ray_index, double dist, t_color color)
{
    t_vert v1;
    t_vert v2;
    double line_height;
    double line_offset;

    line_height = MAP_CELL_LEN * man.tex[man.curr_tex]->size.x / dist;
    if (line_height > man.tex[man.curr_tex]->size.x)
        line_height = man.tex[man.curr_tex]->size.x;
    line_offset = (t->size.y - line_height) / 2;
    v1.color = color;
    v2.color = color;
    v1.coord.x = ray_index;
    v1.coord.y = line_offset;
    v2.coord.x = ray_index;
    v2.coord.y = line_height + line_offset;
    draw_line(t, v1, v2);
    return;
}
