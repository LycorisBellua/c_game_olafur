#include "cimmerian.h"

static int	perform_dda(t_map *m, double cam_x, t_ray *r);
static void	set_line(t_frame *f, int x, t_ray *r);
static int	is_obstacle_see_through(t_map *m, t_ray *r);

void	raycasting(t_map *m)
{
	t_frame	*f;
	int		x;
	t_ray	r;
	double	*z_buffer;

	f = g_man.frame[g_man.curr_frame];
	cast_floor_and_ceiling(f, m);
	z_buffer = calloc(f->size.x, sizeof(double));
	r.alpha = 0;
	x = 0;
	while (x < f->size.x)
	{
		if (perform_dda(m, 2 * x / (double)f->size.x - 1, &r))
		{
			set_line(f, x, &r);
			draw_wall(m, f, &r);
		}
		/*
			TODO:
			For transparency, draw the sprite before drawing the alpha walls.
		*/
		if (z_buffer)
			z_buffer[x] = r.perp_wall_dist;
		while (r.alpha)
		{
			set_line(f, x, r.alpha->data);
			draw_wall(m, f, r.alpha->data);
			list_del_one(&r.alpha, free);
		}
		++x;
	}
	if (z_buffer)
		cast_sprites(f, m, z_buffer);
	free(z_buffer);
	return ;
}

static int	perform_dda(t_map *m, double cam_x, t_ray *r)
{
	t_vec2	delta_dist;
	t_ivec2	step;
	t_vec2	side_dist;
	int		hit;
	t_ray	*alpha;

	r->ray_dir.x = g_man.player.dir.x + g_man.player.plane.x * cam_x;
	r->ray_dir.y = g_man.player.dir.y + g_man.player.plane.y * cam_x;
	r->m_index.x = (int)g_man.player.pos.x;
	r->m_index.y = (int)g_man.player.pos.y;
	delta_dist.x = (r->ray_dir.x == 0) ? 1e30 : f_abs(1 / r->ray_dir.x);
	delta_dist.y = (r->ray_dir.y == 0) ? 1e30 : f_abs(1 / r->ray_dir.y);
	if (r->ray_dir.x < 0)
	{
		step.x = -1;
		side_dist.x = (g_man.player.pos.x - r->m_index.x) * delta_dist.x;
	}
	else
	{
		step.x = 1;
		side_dist.x = (r->m_index.x + 1.0 - g_man.player.pos.x) * delta_dist.x;
	}
	if (r->ray_dir.y < 0)
	{
		step.y = -1;
		side_dist.y = (g_man.player.pos.y - r->m_index.y) * delta_dist.y;
	}
	else
	{
		step.y = 1;
		side_dist.y = (r->m_index.y + 1.0 - g_man.player.pos.y) * delta_dist.y;
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
			return (0);
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
						return (0);
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
		return (0);
	return (1);
}

static void	set_line(t_frame *f, int x, t_ray *r)
{
	r->line_height = (int)(f->size.y / r->perp_wall_dist);
	r->coord1.x = x;
	r->coord1.y = f_max(-r->line_height / 2 + f->size.y / 2, 0);
	r->coord2.x = x;
	r->coord2.y = f_min(r->line_height / 2 + f->size.y / 2, f->size.y - 1);
	return ;
}

static int	is_obstacle_see_through(t_map *m, t_ray *r)
{
	t_cell	*cell;

	cell = &m->cells[r->m_index.y * m->size.x + r->m_index.x];
	if (r->side == 1 && r->ray_dir.y > 0)
		return (cell->tex_north->is_see_through);
	else if (r->side == 1 && r->ray_dir.y < 0)
		return (cell->tex_south->is_see_through);
	else if (r->side == 0 && r->ray_dir.x > 0)
		return (cell->tex_west->is_see_through);
	else if (r->side == 0 && r->ray_dir.x < 0)
		return (cell->tex_east->is_see_through);
	return (0);
}
