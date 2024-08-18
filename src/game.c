#include "cimmerian.h"

static void draw_gradient(t_tex* t);
static void raycasting(void);
static double get_dist(double dist_h, double dist_v, double ray_angle, t_color* color);
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

static void raycasting(void)
{
    t_color color;
    int screen_width, ray_index, map_pos, depth_of_field;
    t_ivec2 map;
    double fov, start_angle, angle_increment, ray_angle;
    double atan, ntan;
    double dist_h, dist_v, dist;
    t_vec2 ray, offset, h, v;

    screen_width = man.tex[man.curr_tex]->size.x;
    fov = RAD_60;
    start_angle = clamp_rad(man.player.angle - fov / 2);
    angle_increment = fov / screen_width;
    for (ray_index = 0; ray_index < screen_width; ++ray_index)
    {
        ray_angle = clamp_rad(start_angle + ray_index * angle_increment);

        /* Check horizontal lines */
        depth_of_field = 0;
        dist_h = 1000000;
        h.x = man.player.pos.x;
        h.y = man.player.pos.y;
        atan = -1 / f_tan(ray_angle);
        if (ray_angle > PI) /* Looking up */
        {
            /* Round to the nearest 64th value */
            ray.y = (((int)man.player.pos.y >> 6) << 6) - 0.0001;
            ray.x = (man.player.pos.y - ray.y) * atan + man.player.pos.x;
            offset.y = -MAP_CELL_LEN;
            offset.x = -offset.y * atan;
        }
        if (ray_angle < PI) /* Looking down */
        {
            /* Round to the nearest 64th value */
            ray.y = (((int)man.player.pos.y >> 6) << 6) + MAP_CELL_LEN;
            ray.x = (man.player.pos.y - ray.y) * atan + man.player.pos.x;
            offset.y = MAP_CELL_LEN;
            offset.x = -offset.y * atan;
        }
        if (ray_angle == 0 || ray_angle == PI) /* Looking straight left or right */
        {
            ray.x = man.player.pos.x;
            ray.y = man.player.pos.y;
            depth_of_field = 8;
        }
        while (depth_of_field < 8)
        {
            map.x = (int)(ray.x) >> 6;
            map.y = (int)(ray.y) >> 6;
            map_pos = map.y * man.map->size.x + map.x;
            if (map_pos > 0 && map_pos < man.map->size.x * man.map->size.y && man.map->data[map_pos] == 1)
            {
                h.x = ray.x;
                h.y = ray.y;
                dist_h = get_distance(man.player.pos, h);
                depth_of_field = 8;
            }
            else
            {
                ray.x += offset.x;
                ray.y += offset.y;
                depth_of_field += 1;
            }
        }

        /* Check vertical lines */
        depth_of_field = 0;
        dist_v = 1000000;
        v.x = man.player.pos.x;
        v.y = man.player.pos.y;
        ntan = -f_tan(ray_angle);
        if (ray_angle > RAD_90 && ray_angle < RAD_270) /* Looking left */
        {
            /* Round to the nearest 64th value */
            ray.x = (((int)man.player.pos.x >> 6) << 6) - 0.0001;
            ray.y = (man.player.pos.x - ray.x) * ntan + man.player.pos.y;
            offset.x = -MAP_CELL_LEN;
            offset.y = -offset.x * ntan;
        }
        if (ray_angle < RAD_90 || ray_angle > RAD_270) /* Looking right */
        {
            /* Round to the nearest 64th value */
            ray.x = (((int)man.player.pos.x >> 6) << 6) + MAP_CELL_LEN;
            ray.y = (man.player.pos.x - ray.x) * ntan + man.player.pos.y;
            offset.x = MAP_CELL_LEN;
            offset.y = -offset.x * ntan;
        }
        if (ray_angle == 0 || ray_angle == PI) /* Looking straight up or down */
        {
            ray.x = man.player.pos.x;
            ray.y = man.player.pos.y;
            depth_of_field = 8;
        }
        while (depth_of_field < 8)
        {
            map.x = (int)(ray.x) >> 6;
            map.y = (int)(ray.y) >> 6;
            map_pos = map.y * man.map->size.x + map.x;
            if (map_pos > 0 && map_pos < man.map->size.x * man.map->size.y && man.map->data[map_pos] == 1)
            {
                v.x = ray.x;
                v.y = ray.y;
                dist_v = get_distance(man.player.pos, v);
                depth_of_field = 8;
            }
            else
            {
                ray.x += offset.x;
                ray.y += offset.y;
                depth_of_field += 1;
            }
        }

        dist = get_dist(dist_h, dist_v, ray_angle, &color);
        draw_wall(man.tex[man.curr_tex], ray_index, dist, color);
    }
    return;
}

static double get_dist(double dist_h, double dist_v, double ray_angle, t_color* color)
{
    double dist;
    double center_angle;

    /* Get shortest dist */
    if (dist_h < dist_v)
    {
        dist = dist_h;
        *color = get_color_rgba(74, 42, 77, 255);
    }
    else
    {
        dist = dist_v;
        *color = get_color_rgba(84, 43, 88, 255);
    }
    /* Fix fisheye effect */
    center_angle = clamp_rad(man.player.angle - ray_angle);
    dist *= f_cos(center_angle);
    return (dist);
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
