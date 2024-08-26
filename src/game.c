#include "cimmerian.h"

static int perform_dda(t_map* m, double cam_x, t_ray* r);
static void set_line(t_tex* t, int x, t_ray *r);
static void wall_texturing(t_map* m, t_tex* t, int x, t_ray* r);

static void raycasting(t_map* m);

void draw_game(t_map* m)
{
    draw_floor_gradient(man.tex[man.curr_tex], m->fog_width, m->fog_color);
    draw_ceiling_gradient(man.tex[man.curr_tex], m->fog_width, m->fog_color);
    raycasting(m);
    door_routine(m);
    return;
}

static void raycasting(t_map* m)
{
    t_tex *t;
    int x;
    t_ray r;

    t = man.tex[man.curr_tex];
    r.alpha = 0;
    x = 0;
    while (x < t->size.x)
    {
        if (perform_dda(m, 2 * x / (double)t->size.x - 1, &r))
        {
            set_line(t, x, &r);
            wall_texturing(m, t, x, &r);
        }
        while (r.alpha)
        {
            set_line(t, x, r.alpha->data);
            wall_texturing(m, t, x, r.alpha->data);
            list_del_one(&r.alpha, basic_free);
        }
        ++x;
    }
    return;
}

static int perform_dda(t_map* m, double cam_x, t_ray* r)
{
    t_vec2 delta_dist;
    t_ivec2 step;
    t_vec2 side_dist;
    int hit;
    t_ray* alpha;

    r->ray_dir.x = man.player.dir.x + man.player.plane.x * cam_x;
    r->ray_dir.y = man.player.dir.y + man.player.plane.y * cam_x;
    r->m_index.x = (int)man.player.pos.x;
    r->m_index.y = (int)man.player.pos.y;
    delta_dist.x = (r->ray_dir.x == 0) ? 1e30 : f_abs(1 / r->ray_dir.x);
    delta_dist.y = (r->ray_dir.y == 0) ? 1e30 : f_abs(1 / r->ray_dir.y);
    if (r->ray_dir.x < 0)
    {
        step.x = -1;
        side_dist.x = (man.player.pos.x - r->m_index.x) * delta_dist.x;
    }
    else
    {
        step.x = 1;
        side_dist.x = (r->m_index.x + 1.0 - man.player.pos.x) * delta_dist.x;
    }
    if (r->ray_dir.y < 0)
    {
        step.y = -1;
        side_dist.y = (man.player.pos.y - r->m_index.y) * delta_dist.y;
    }
    else
    {
        step.y = 1;
        side_dist.y = (r->m_index.y + 1.0 - man.player.pos.y) * delta_dist.y;
    }
    hit = 0;
    while (!hit)
    {
        if (side_dist.x < side_dist.y)
        {
            side_dist.x += delta_dist.x;
            r->m_index.x += step.x;
            r->side = 0;
        }
        else
        {
            side_dist.y += delta_dist.y;
            r->m_index.y += step.y;
            r->side = 1;
        }
        if (r->m_index.x < 0 || r->m_index.y < 0 
            || r->m_index.x >= m->size.x || r->m_index.y >= m->size.y)
            return 0;
        else if (m->data[r->m_index.y * m->size.x + r->m_index.x] > 0)
        {
            if (m->data[r->m_index.y * m->size.x + r->m_index.x] == 5)
            {
                alpha = malloc(sizeof(t_ray));
                if (alpha)
                {
                    alpha->side = r->side;
                    alpha->m_index = r->m_index;
                    alpha->ray_dir = r->ray_dir;
                    if (alpha->side == 0)
                        alpha->perp_wall_dist = (side_dist.x - delta_dist.x);
                    else
                        alpha->perp_wall_dist = (side_dist.y - delta_dist.y);
                    list_add_front(&r->alpha, list_new(alpha));
                }
            }
            else
                hit = 1;
        }
    }
    if (r->side == 0)
        r->perp_wall_dist = (side_dist.x - delta_dist.x);
    else
        r->perp_wall_dist = (side_dist.y - delta_dist.y);
    return 1;
}

static void set_line(t_tex* t, int x, t_ray *r)
{
    r->line_height = (int)(t->size.y / r->perp_wall_dist * man.res.h_mod);
    r->coord1.x = x;
    r->coord1.y = -r->line_height / 2 + t->size.y / 2 + man.player.height;
    if (r->coord1.y < 0)
        r->coord1.y = 0;
    r->coord2.x = x;
    r->coord2.y = r->line_height / 2 + t->size.y / 2 + man.player.height;
    if (r->coord2.y >= t->size.y)
        r->coord2.y = t->size.y - 1;
    return;
}

static void wall_texturing(t_map* m, t_tex* t, int x, t_ray* r)
{
    int s_num;
    s_num = m->data[r->m_index.y * m->size.x + r->m_index.x] - 1;

    // Where exactly the wall was hit
    double wall_x;
    if (r->side == 0)
        wall_x = man.player.pos.y + r->perp_wall_dist * r->ray_dir.y;
    else
        wall_x = man.player.pos.x + r->perp_wall_dist * r->ray_dir.x;
    wall_x -= f_floor(wall_x);

    // X coordinate on the sprite
    t_ivec2 s_coord;
    s_coord.x = (int)(wall_x * (double)SPR_W);
    if ((r->side == 0 && r->ray_dir.x < 0)
        || (r->side == 1 && r->ray_dir.y > 0))
        s_coord.x = SPR_W - s_coord.x - 1;

    // How much to increase the sprite coordinate per screen pixel
    double s_step = (double)SPR_H / (double)r->line_height;

    // Starting sprite coordinate
    double s_pos = (r->coord1.y - (t->size.y / 2.0 - r->line_height / 2.0)) * s_step;

    t_color* s_buf = (t_color*)man.map->spr[s_num]->buf;
    t_color color;
    int y = r->coord1.y;
    while (y < r->coord2.y)
    {
        // Cast the sprite coordinate to integer, and clamp to [0, SPR_H - 1]
        s_coord.y = (int)s_pos;
        if (s_coord.y < 0) s_coord.y = 0;
        if (s_coord.y >= SPR_H) s_coord.y = SPR_H - 1;
        s_pos += s_step;

        color = s_buf[s_coord.y * SPR_W + s_coord.x];

        // Make color darker for y-sides
        if (r->side == 1)
        {
            color.r /= 2;
            color.g /= 2;
            color.b /= 2;
        }
        apply_wall_fog(&color, m->fog_color, r->perp_wall_dist, m->dof);
        draw_point(t, color, x, y);
        ++y;
    }
    return;
}
