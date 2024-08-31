#include "cimmerian.h"

static void raycasting(t_map* m);
static int perform_dda(t_map* m, double cam_x, t_ray* r);
static void set_line(t_frame* f, int x, t_ray *r);
static int is_obstacle_see_through(t_map* m, t_ray* r);

void draw_game(t_map* m)
{
    draw_floor(man.frame[man.curr_frame], m->fog_width, m->fog_color);
    draw_ceiling(man.frame[man.curr_frame], m->fog_width, m->fog_color);
    raycasting(m);
    door_routine(m);
    return;
}

static void raycasting(t_map* m)
{
    t_frame *f;
    int x;
    t_ray r;

    f = man.frame[man.curr_frame];
    r.alpha = 0;
    x = 0;
    while (x < f->size.x)
    {
        if (perform_dda(m, 2 * x / (double)f->size.x - 1, &r))
        {
            set_line(f, x, &r);
            draw_wall(m, f, &r);
        }
        while (r.alpha)
        {
            set_line(f, x, r.alpha->data);
            draw_wall(m, f, r.alpha->data);
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
        else if (m->cells[r->m_index.y * m->size.x + r->m_index.x].is_obstacle)
        {
            if (!is_obstacle_see_through(m, r))
                hit = 1;
            else
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
                    if (alpha->perp_wall_dist > m->dof)
                    {
                        free(alpha);
                        return 0;
                    }
                    list_add_front(&r->alpha, list_new(alpha));
                }
            }
        }
    }
    if (r->side == 0)
        r->perp_wall_dist = (side_dist.x - delta_dist.x);
    else
        r->perp_wall_dist = (side_dist.y - delta_dist.y);
    if (r->perp_wall_dist > m->dof)
        return 0;
    return 1;
}

static void set_line(t_frame* f, int x, t_ray *r)
{
    r->line_height = (int)(f->size.y / r->perp_wall_dist * man.res.h_mod);
    r->coord1.x = x;
    r->coord1.y = -r->line_height / 2 + f->size.y / 2 + man.player.height;
    if (r->coord1.y < 0)
        r->coord1.y = 0;
    r->coord2.x = x;
    r->coord2.y = r->line_height / 2 + f->size.y / 2 + man.player.height;
    if (r->coord2.y >= f->size.y)
        r->coord2.y = f->size.y - 1;
    return;
}

static int is_obstacle_see_through(t_map* m, t_ray* r)
{
    t_cell* cell;

    cell = &m->cells[r->m_index.y * m->size.x + r->m_index.x];
    if (r->side == 1 && r->ray_dir.y > 0)
        return cell->tex_north->is_see_through;
    else if (r->side == 1 && r->ray_dir.y < 0)
        return cell->tex_south->is_see_through;
    else if (r->side == 0 && r->ray_dir.x > 0)
        return cell->tex_west->is_see_through;
    else if (r->side == 0 && r->ray_dir.x < 0)
        return cell->tex_east->is_see_through;
    return 0;
}
