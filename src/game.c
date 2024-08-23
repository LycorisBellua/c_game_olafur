#include "cimmerian.h"

static void raycasting(t_map* m);
void check_door_activation(t_map* m);

void draw_game(t_map* m)
{
    draw_floor_gradient(man.tex[man.curr_tex], m->fog_width, m->fog_color);
    draw_ceiling_gradient(man.tex[man.curr_tex], m->fog_width, m->fog_color);
    raycasting(m);
    check_door_activation(m);
    return;
}

void check_door_activation(t_map* m)
{
    static t_ivec2 last_door = { -1, -1 };
    int x = (int)man.player.pos.x;
    int y = (int)man.player.pos.y;

    /*
        TODO:
        Do not just use the distance from the door.
        If the player is not exactly in the cells that would trigger the 
        opening of the door, close it.
        Take a look at the grey room for example. The player runs from enemies, 
        enters the room, and hide in a corner diagonal to the door so it can 
        close, and the enemies cannot open the door for whatever reason (plot). 
        But currently, the door doesn't close because the player isn't far 
        enough.

        Anyway, there's also the fact that the function is way too long. Should 
        I use the Manhattan distance?
    */
    if (last_door.x >= 0)
    {
        t_ivec2 diff;
        diff.x = abs(x - last_door.x);
        diff.y = abs(y - last_door.y);
        if (diff.x > 3 || diff.y > 3)
        {
            m->data[last_door.y * m->size.x + last_door.x] = 5;
            last_door.x = -1;
            last_door.y = -1;
        }
    }

    if (m->data[y * m->size.x + (x - 1)] == 5)
    {
        //printf("Door is to the left\n");
        last_door.x = x - 1;
        last_door.y = y;
        m->data[last_door.y * m->size.x + last_door.x] = 0;
    }
    else if (m->data[y * m->size.x + (x - 2)] == 5)
    {
        //printf("Door is to the left\n");
        last_door.x = x - 2;
        last_door.y = y;
        m->data[last_door.y * m->size.x + last_door.x] = 0;
    }
    else if (m->data[y * m->size.x + (x + 1)] == 5)
    {
        //printf("Door is to the right\n");
        last_door.x = x + 1;
        last_door.y = y;
        m->data[last_door.y * m->size.x + last_door.x] = 0;
    }
    else if (m->data[y * m->size.x + (x + 2)] == 5)
    {
        //printf("Door is to the right\n");
        last_door.x = x + 2;
        last_door.y = y;
        m->data[last_door.y * m->size.x + last_door.x] = 0;
    }
    else if (m->data[(y - 1) * m->size.x + x] == 5)
    {
        //printf("Door is in front\n");
        last_door.x = x;
        last_door.y = y - 1;
        m->data[last_door.y * m->size.x + last_door.x] = 0;
    }
    else if (m->data[(y - 2) * m->size.x + x] == 5)
    {
        //printf("Door is in front\n");
        last_door.x = x;
        last_door.y = y - 2;
        m->data[last_door.y * m->size.x + last_door.x] = 0;
    }
    else if (m->data[(y + 1) * m->size.x + x] == 5)
    {
        //printf("Door is behind\n");
        last_door.x = x;
        last_door.y = y + 1;
        m->data[last_door.y * m->size.x + last_door.x] = 0;
    }
    else if (m->data[(y + 2) * m->size.x + x] == 5)
    {
        //printf("Door is behind\n");
        last_door.x = x;
        last_door.y = y + 2;
        m->data[last_door.y * m->size.x + last_door.x] = 0;
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
        double camera_x;
        camera_x = 2 * x / (double)t->size.x - 1;

        t_vec2 ray_dir;
        ray_dir.x = man.player.dir.x + man.player.plane.x * camera_x;
        ray_dir.y = man.player.dir.y + man.player.plane.y * camera_x;

        t_ivec2 m_index;
        m_index.x = (int)man.player.pos.x;
        m_index.y = (int)man.player.pos.y;

        t_vec2 delta_dist;
        delta_dist.x = (ray_dir.x == 0) ? 1e30 : f_abs(1 / ray_dir.x);
        delta_dist.y = (ray_dir.y == 0) ? 1e30 : f_abs(1 / ray_dir.y);

        t_ivec2 step;
        t_vec2 side_dist;
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

        int hit;
        int side;
        hit = 0;
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
        v1.coord.y = -line_height / 2 + t->size.y / 2 + man.player.height;
        if (v1.coord.y < 0)
            v1.coord.y = 0;
        v2.coord.x = x;
        v2.coord.y = line_height / 2 + t->size.y / 2 + man.player.height;
        if (v2.coord.y >= t->size.y)
            v2.coord.y = t->size.y - 1;

        /* Wall Texturing --------------------------------------------------- */

        int s_num;
        
        s_num = m->data[m_index.y * m->size.x + m_index.x] - 1;
        /*
        if (side == 0 && ray_dir.x > 0) // WEST
            s_num = 0;
        else if (side == 0 && ray_dir.x < 0) // EAST
            s_num = 1;
        else if (side == 1 && ray_dir.y > 0) // NORTH
            s_num = 2;
        else if (side == 1 && ray_dir.y < 0) // SOUTH
            s_num = 3;
        s_num = s_num % man.map->spr_len;
        */

        // Where exactly the wall was hit
        double wall_x;
        if (side == 0)
            wall_x = man.player.pos.y + perp_wall_dist * ray_dir.y;
        else
            wall_x = man.player.pos.x + perp_wall_dist * ray_dir.x;
        wall_x -= f_floor((wall_x));


        // X coordinate on the sprite
        t_ivec2 s_coord;
        s_coord.x = (int)(wall_x * (double)SPR_W);
        if ((side == 0 && ray_dir.x < 0) || (side == 1 && ray_dir.y > 0))
            s_coord.x = SPR_W - s_coord.x - 1;

        // How much to increase the sprite coordinate per screen pixel
        double s_step;
        s_step = 1.0 * SPR_H / line_height;

        // Starting sprite coordinate
        double s_pos;
        s_pos = (v1.coord.y - man.player.height - t->size.y / 2 
            + line_height / 2) * s_step;

        t_color* s_buf;
        t_color color;
        int y;
        s_buf = (t_color*)man.map->spr[s_num]->buf;
        y = v1.coord.y;
        while (y < v2.coord.y)
        {
            // Cast the sprite coordinate to integer, and mask with (SPR_H - 1) 
            // in case of overflow
            s_coord.y = (int)s_pos & (SPR_H - 1);
            s_pos += s_step;
            color = s_buf[s_coord.y * SPR_H + s_coord.x];
            // Make color darker for y-sides
            if (side == 1)
            {
                color.r -= color.r / 2;
                color.g -= color.g / 2;
                color.b -= color.b / 2;
            }
            apply_wall_fog(&color, m->fog_color, perp_wall_dist, m->dof);
            if (s_num == 4)
                color.a = 255/5;
            draw_point(t, color, x, y);
            ++y;
        }
    }
    return;
}
