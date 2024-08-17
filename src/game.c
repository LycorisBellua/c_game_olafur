#include "cimmerian.h"

#define FOV 60
#define WALL_HEIGHT 35456
/*
    Logic for WALL_HEIGHT is `MAP_CELL_LEN * dist_proj_plane`.
    Second value is `man.tex[man.curr_tex]->size.x / (2.0 * f_tan(DEG2RAD(FOV)/2))`.
    In other words: `WALL_HEIGHT = 64 * 554`.
*/

static int is_player_out_of_bounds(t_vec2 pos, t_map* m);

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
static void draw_gradient(t_tex* t);

/*
    (float) px  -> (double) man.player.pos.x
    (float) py  -> (double) man.player.pos.y
    (float) pdx -> (double) man.player.delta.x
    (float) pdy -> (double) man.player.delta.y
    (float) pa  -> (double) man.player.angle

    (int) r   -> (int) "ray"
    (int) mx  -> (int)
    (int) my  -> (int)
    (int) mp  -> (int) 
    (int) dof -> (int) "depth of field"
    (float) rx  -> (double) "ray x"
    (float) ry  -> (double) "ray y"
    (float) ra  -> (double) "ray angle"
    (float) xo  -> (double) "x offset"
    (float) yo  -> (double) "y offset"
    (float) atan -> (double) "arc tangent"
    (float) ntan -> (double) "negative tangent"
    (float) disH -> (double) "distance horizontal"
    (float) hx and hy -> (double)
    (float) disV -> (double) "distance vertical"
    (float) vx and vy -> (double)
    (float) disT -> (double) "final distance"
    (float) lineH -> (double) "line height"
*/

double dist(double ax, double ay, double bx, double by, double ang)
{
    return f_sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay));
}

void draw_wall_line(t_tex* texture, int ray, int line_height, t_color color)
{
    t_vert v1, v2;
    int i;

    /* Calculate the base x-coordinate for the ray */
    int base_x = ray * 8;
    float line_offset = texture->size.y - line_height / 2;

    v1.color = color;
    v2.color = color;
    v1.coord.y = line_offset;
    v2.coord.y = line_height + line_offset;

    /* Loop to draw 8 parallel lines to simulate thickness */
    for (i = 0; i < 8; ++i)
    {
        v1.coord.x = base_x + i;
        v2.coord.x = base_x + i;
        draw_line(texture, v1, v2);
    }
}

void draw_game(void)
{
    double speed;
    t_vec2 pos;
    t_ivec2 i;
    t_vert vert, vert2;
    t_ivec2 size;
    int r, mx, my, mp, dof;
    double rx, ry, ra, xo, yo, atan, ntan, disH, hx, hy, disV, vx, vy, disT, lineH;

    draw_gradient(man.tex[man.curr_tex]);

    /* PLAYER ROTATION */
    if (man.rotation_action)
    {
        speed = 30.0;
        man.player.angle += man.rotation_action * RAD_1 * speed * man.delta_time;
        man.player.angle = clamp_rad(man.player.angle);
        man.player.delta.x = f_cos(man.player.angle) * 5;
        man.player.delta.y = f_sin(man.player.angle) * 5;
    }
    /* PLAYER POSITION */
    if (man.movement_action[2] + man.movement_action[0] != 0)
    {
        speed = 15.0;
        pos = man.player.pos;

        /* Movement along the forward axis */
        pos.x += man.movement_action[2] * man.player.delta.x * speed * man.delta_time;
        pos.y += man.movement_action[2] * man.player.delta.y * speed * man.delta_time;

        /* Movement along the lateral axis */
        pos.x += man.movement_action[0] * man.player.delta.y * speed * man.delta_time;
        pos.y += man.movement_action[0] * -man.player.delta.x * speed * man.delta_time;

        man.player.pos = pos;
    }

    /* DRAW MAP */
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
            size.x = 64;
            size.y = 64;
            vert.coord.x = i.x * size.x;
            vert.coord.y = i.y * size.y;
            --size.x;
            --size.y;
            draw_rectangle_full(man.tex[man.curr_tex], vert, size);
            ++i.x;
        }
        ++i.y;
    }

    /* DRAW PLAYER */
    /* TODO: Put the line in the middle of the square */
    vert.color = get_color_rgba(0x00, 0xFF, 0xFF, 0xFF);
    vert.coord.x = man.player.pos.x;
    vert.coord.y = man.player.pos.y;
    vert2.color = get_color_rgba(0xFF, 0x00, 0x00, 0xFF);
    vert2.coord.x = vert.coord.x + man.player.delta.x * 5;
    vert2.coord.y = vert.coord.y + man.player.delta.y * 5;
    draw_line(man.tex[man.curr_tex], vert, vert2);
    size.x = 8;
    size.y = 8;
    draw_rectangle_full(man.tex[man.curr_tex], vert, size);
    vert.color = get_color_rgba(0x00, 0x00, 0x00, 0xFF);
    draw_rectangle(man.tex[man.curr_tex], vert, size);

    /* RAYCASTING */
    /* Note that the cells are 64x64 */
    ra = clamp_rad(man.player.angle - RAD_1 * 30);
    for (r = 0; r < 60; ++r)
    {
        /* Check horizontal lines */
        dof = 0;
        disH = 1000000;
        hx = man.player.pos.x;
        hy = man.player.pos.y;
        atan = -1 / f_tan(ra);
        if (ra > PI) /* Looking up */
        {
            /* Round to the nearest 64th value */
            ry = (((int)man.player.pos.y >> 6) << 6) - 0.0001;
            rx = (man.player.pos.y - ry) * atan + man.player.pos.x;
            yo = -64;
            xo = -yo * atan;
        }
        if (ra < PI) /* Looking down */
        {
            /* Round to the nearest 64th value */
            ry = (((int)man.player.pos.y >> 6) << 6) + 64;
            rx = (man.player.pos.y - ry) * atan + man.player.pos.x;
            yo = 64;
            xo = -yo * atan;
        }
        if (ra == 0 || ra == PI) /* Looking straight left or right */
        {
            rx = man.player.pos.x;
            ry = man.player.pos.y;
            dof = 8;
        }
        while (dof < 8)
        {
            mx = (int)(rx) >> 6;
            my = (int)(ry) >> 6;
            mp = my * man.map->size.x + mx;
            if (mp > 0 && mp < man.map->size.x * man.map->size.y && man.map->data[mp] == 1)
            {
                hx = rx;
                hy = ry;
                disH = dist(man.player.pos.x, man.player.pos.y, hx, hy, ra);
                dof = 8;
            }
            else
            {
                rx += xo;
                ry += yo;
                dof += 1;
            }
        }

        /* Check vertical lines */
        dof = 0;
        disV = 1000000;
        vx = man.player.pos.x;
        vy = man.player.pos.y;
        ntan = -f_tan(ra);
        if (ra > RAD_90 && ra < RAD_270) /* Looking left */
        {
            /* Round to the nearest 64th value */
            rx = (((int)man.player.pos.x >> 6) << 6) - 0.0001;
            ry = (man.player.pos.x - rx) * ntan + man.player.pos.y;
            xo = -64;
            yo = -xo * ntan;
        }
        if (ra < RAD_90 || ra > RAD_270) /* Looking right */
        {
            /* Round to the nearest 64th value */
            rx = (((int)man.player.pos.x >> 6) << 6) + 64;
            ry = (man.player.pos.x - rx) * ntan + man.player.pos.y;
            xo = 64;
            yo = -xo * ntan;
        }
        if (ra == 0 || ra == PI) /* Looking straight up or down */
        {
            rx = man.player.pos.x;
            ry = man.player.pos.y;
            dof = 8;
        }
        while (dof < 8)
        {
            mx = (int)(rx) >> 6;
            my = (int)(ry) >> 6;
            mp = my * man.map->size.x + mx;
            if (mp > 0 && mp < man.map->size.x * man.map->size.y && man.map->data[mp] == 1)
            {
                vx = rx;
                vy = ry;
                disV = dist(man.player.pos.x, man.player.pos.y, vx, vy, ra);
                dof = 8;
            }
            else
            {
                rx += xo;
                ry += yo;
                dof += 1;
            }
        }

        /* Shortest distance between horizontal and vertical */
        if (disV < disH)
        {
            rx = vx;
            ry = vy;
            disT = disV;
        }
        if (disH < disV)
        {
            rx = hx;
            ry = hy;
            disT = disH;
        }
        vert.color = get_color_rgba(0xFF, 0x00, 0x00, 0xFF);
        vert.coord.x = man.player.pos.x;
        vert.coord.y = man.player.pos.y;
        vert2.color = vert.color;
        vert2.coord.x = rx;
        vert2.coord.y = ry;
        draw_line(man.tex[man.curr_tex], vert, vert2);

        /* Draw 3D walls */
        lineH = 64 * man.tex[man.curr_tex]->size.x / disT;
        if (lineH > man.tex[man.curr_tex]->size.x)
            lineH = man.tex[man.curr_tex]->size.x;
        draw_wall_line(man.tex[man.curr_tex], r, lineH, vert.color);

        ra = clamp_rad(ra + RAD_1);
    }
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
