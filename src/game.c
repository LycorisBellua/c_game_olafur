#include "cimmerian.h"

static void raycasting(t_map* m);

void draw_game(t_map* m)
{
    draw_floor_gradient(man.tex[man.curr_tex], m->fog_width, m->fog_color);
    draw_ceiling_gradient(man.tex[man.curr_tex], m->fog_width, m->fog_color);
    raycasting(m);
    return;
}

static void raycasting(t_map* m)
{
    t_tex *t;
    int x;

    t_spr* texture[5];
    texture[0] = load_sprite("pics/bluestone.png");
    texture[1] = load_sprite("pics/greystone.png");
    texture[2] = load_sprite("pics/purplestone.png");
    texture[3] = load_sprite("pics/redbrick.png");
    texture[4] = load_sprite("pics/mossy.png");
    /*
        `texture` is an array of 5 t_spr pointers.
        Each of size 64x64.
    */

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
        v1.coord.y = -line_height / 2 + t->size.y / 2 + man.player.height;
        if (v1.coord.y < 0)
            v1.coord.y = 0;
        v2.coord.x = x;
        v2.coord.y = line_height / 2 + t->size.y / 2 + man.player.height;
        if (v2.coord.y >= t->size.y)
            v2.coord.y = t->size.y - 1;

        //texturing calculations
        int texNum;
        
        //1 subtracted from it so that texture 0 can be used!
        //texNum = m->data[m_index.y * m->size.x + m_index.x] - 1;

        if (side == 0 && ray_dir.x > 0) // WEST
            texNum = 0;
        else if (side == 0 && ray_dir.x < 0) // EAST
            texNum = 1;
        else if (side == 1 && ray_dir.y > 0) // NORTH
            texNum = 2;
        else if (side == 1 && ray_dir.y < 0) // SOUTH
            texNum = 3;

        //calculate value of wallX
        double wallX; //where exactly the wall was hit
        if(side == 0) wallX = man.player.pos.y + perp_wall_dist * ray_dir.y;
        else          wallX = man.player.pos.x + perp_wall_dist * ray_dir.x;
        wallX -= f_floor((wallX));

        int texWidth = 64;
        int texHeight = 64;
        //x coordinate on the texture
        int texX = (int)(wallX * (double)texWidth);
        if(side == 0 && ray_dir.x > 0) texX = texWidth - texX - 1;
        if(side == 1 && ray_dir.y < 0) texX = texWidth - texX - 1;

        // TODO: an integer-only bresenham or DDA like algorithm could make the texture coordinate stepping faster
        // How much to increase the texture coordinate per screen pixel
        double texStep = 1.0 * texHeight / line_height;
        // Starting texture coordinate
        double texPos = (v1.coord.y - man.player.height - t->size.y / 2 + line_height / 2) * texStep;
        for(int y = v1.coord.y; y < v2.coord.y; y++)
        {
            // Cast the texture coordinate to integer, and mask with (texHeight - 1) in case of overflow
            int texY = (int)texPos & (texHeight - 1);
            texPos += texStep;
            t_color color = *((t_color*)texture[texNum]->buf + (texY * texHeight + texX));
            //make color darker for y-sides:
            if (side == 1)
            {
                color.r -= color.r / 2;
                color.g -= color.g / 2;
                color.b -= color.b / 2;
            }
            apply_wall_fog(&color, m->fog_color, perp_wall_dist, m->dof);
            draw_point(t, color, x, y);
        }

        /*
        t_color color;
        if (side == 0 && ray_dir.x > 0) // WEST
            color = get_color_rgba( 93,  42,  98, 255); //purple
        else if (side == 0 && ray_dir.x < 0) // EAST
            color = get_color_rgba( 78, 120,  94, 255); //green
        else if (side == 1 && ray_dir.y > 0) // NORTH
            color = get_color_rgba( 83, 120, 156, 255); //blue
        else if (side == 1 && ray_dir.y < 0) // SOUTH
            color = get_color_rgba(155, 114,  44, 255); //yellow
        */
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
        /*
        //give x and y sides different brightness
        if (side == 1)
        {
            color.r -= color.r / 5;
            color.g -= color.g / 5;
            color.b -= color.b / 5;
        }
        apply_wall_fog(&color, m->fog_color, perp_wall_dist, m->dof);
        v1.color = color;
        v2.color = color;
        draw_line(t, v1, v2);
        */
    }
    return;
}
