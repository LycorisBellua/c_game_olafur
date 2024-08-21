#include "cimmerian.h"

static void draw_ceiling_gradient(t_tex* t);
static void draw_floor_gradient(t_tex* t);
static void raycasting(t_map* m);

void draw_game(t_map* m)
{
    draw_ceiling_gradient(man.tex[man.curr_tex]);
    draw_floor_gradient(man.tex[man.curr_tex]);
    raycasting(m);
    return;
}

static void draw_ceiling_gradient(t_tex* t)
{
    t_vert v;
    t_color top;
    t_color bottom;
    float factor;

    top = get_color_rgba(64, 0, 64, 255);
    bottom = get_color_rgba(0, 0, 0, 255);
    v.coord.y = 0;
    while (v.coord.y < t->size.y / 2)
    {
        v.coord.x = 0;
        while (v.coord.x < t->size.x)
        {
            factor = (float)v.coord.y / (t->size.y / 2 - 1);
            v.color.r = top.r + factor * (bottom.r - top.r);
            v.color.g = top.g + factor * (bottom.g - top.g);
            v.color.b = top.b + factor * (bottom.b - top.b);
            v.color.a = 255;
            draw_point(t, v.color, v.coord.x, v.coord.y);
            ++v.coord.x;
        }
        ++v.coord.y;
    }
    return;
}

static void draw_floor_gradient(t_tex* t)
{
    t_vert v;
    t_color top;
    t_color bottom;
    float factor;

    top = get_color_rgba(0, 0, 0, 255);
    bottom = get_color_rgba(42, 30, 30, 255);
    v.coord.y = t->size.y / 2;
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
    return;
}

static void raycasting(t_map* m)
{
    t_tex *t;
    int x;

    t = man.tex[man.curr_tex];

    for (x = 0; x < t->size.x; ++x)
    {
        double camera_x = 2 * x / (double)t->size.x - 1;
        t_vec2 ray_dir;
        ray_dir.x = man.player.dir.x + man.player.plane.x * camera_x;
        ray_dir.y = man.player.dir.y + man.player.plane.y * camera_x;
        t_ivec2 m_index;
        m_index.x = (int)man.player.pos.x;
        m_index.y = (int)man.player.pos.y;

        t_vec2 side_dist;

        t_vec2 delta_dist;
        delta_dist.x = (ray_dir.x == 0) ? 1e30 : f_abs(1 / ray_dir.x);
        delta_dist.y = (ray_dir.y == 0) ? 1e30 : f_abs(1 / ray_dir.y);

        t_ivec2 step;
        int hit = 0;
        int side;
        if (ray_dir.x < 0)
        {
            step.x = -1;
            side_dist.x = (man.player.pos.x - m_index.x) * delta_dist.x;
        }
        else
        {
            step.x = 1;
            side_dist.x = (m_index.x + 1.0 - man.player.pos.x) * delta_dist.x;
        }
        if (ray_dir.y < 0)
        {
            step.y = -1;
            side_dist.y = (man.player.pos.y - m_index.y) * delta_dist.y;
        }
        else
        {
            step.y = 1;
            side_dist.y = (m_index.y + 1.0 - man.player.pos.y) * delta_dist.y;
        }
        while (!hit)
        {
            if (side_dist.x < side_dist.y)
            {
                side_dist.x += delta_dist.x;
                m_index.x += step.x;
                side = 0;
            }
            else
            {
                side_dist.y += delta_dist.y;
                m_index.y += step.y;
                side = 1;
            }
            if (m->data[m_index.y * m->size.x + m_index.x] > 0)
                hit = 1;
        }
        double perp_wall_dist;
        if (side == 0)
            perp_wall_dist = (side_dist.x - delta_dist.x);
        else
            perp_wall_dist = (side_dist.y - delta_dist.y);

        int line_height = (int)(t->size.y / perp_wall_dist * man.res.h_mod);
        t_vert v1;
        t_vert v2;
        v1.coord.x = x;
        v1.coord.y = -line_height / 2 + t->size.y / 2;
        if (v1.coord.y < 0)
            v1.coord.y = 0;
        v2.coord.x = x;
        v2.coord.y = line_height / 2 + t->size.y / 2;
        if (v2.coord.y >= t->size.y)
            v2.coord.y = t->size.y - 1;

        t_color color;
        if (side == 0 && ray_dir.x > 0) // WEST
            color = get_color_rgba( 93,  42,  98, 255); //purple
        else if (side == 0 && ray_dir.x < 0) // EAST
            color = get_color_rgba( 78, 120,  94, 255); //green
        else if (side == 1 && ray_dir.y > 0) // NORTH
            color = get_color_rgba( 83, 120, 156, 255); //blue
        else if (side == 1 && ray_dir.y < 0) // SOUTH
            color = get_color_rgba(155, 114,  44, 255); //yellow
        /*
        if (m->data[m_index.y * m->size.x + m_index.x] == 1)
            color = get_color_rgba(93, 42, 98, 255); //purple
        else if (m->data[m_index.y * m->size.x + m_index.x] == 2)
            color = get_color_rgba(78, 120, 94, 255); //green
        else if (m->data[m_index.y * m->size.x + m_index.x] == 3)
            color = get_color_rgba(83, 120, 156, 255); //blue
        else if (m->data[m_index.y * m->size.x + m_index.x] == 4)
            color = get_color_rgba(155, 114, 44, 255); //yellow
        else
            color = get_color_rgba(255, 255, 255, 255); //white
        */
        //give x and y sides different brightness
        if (side == 1)
        {
            color.r -= color.r / 5;
            color.g -= color.g / 5;
            color.b -= color.b / 5;
        }
        v1.color = color;
        v2.color = color;
        draw_line(t, v1, v2);
    }
    return;
}
