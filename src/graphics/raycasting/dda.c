#include "cimmerian.h"

static int	is_obstacle_see_through(t_map *m, t_ray *r);

void	perform_dda(t_map *m, double cam_x, t_list **list)
{
	t_vec2	delta_dist;
	t_ivec2	step;
	t_vec2	side_dist;
	t_ray	*r;
	double	biggest_height;

	r = calloc(1, sizeof(t_ray));
	if (!r)
		return ;
	r->ray_dir.x = g_man.player.dir.x + g_man.player.plane.x * cam_x;
	r->ray_dir.y = g_man.player.dir.y + g_man.player.plane.y * cam_x;
	r->m_index.x = (int)g_man.player.pos.x;
	r->m_index.y = (int)g_man.player.pos.y;
	delta_dist.x = (r->ray_dir.x == 0) ? 1e30 : abs_f(1 / r->ray_dir.x);
	delta_dist.y = (r->ray_dir.y == 0) ? 1e30 : abs_f(1 / r->ray_dir.y);
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
	biggest_height = 0;
	while (1)
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
			break ;
		else if (m->cells[r->m_index.y * m->size.x + r->m_index.x].is_obstacle)
		{
			t_cell	*cell = &m->cells[r->m_index.y * m->size.x + r->m_index.x];
			int	add_to_list = 0;
			r->is_see_through = is_obstacle_see_through(m, r);

			if (r->is_see_through)
				add_to_list = 1;
			else if (cell->height > biggest_height)
			{
				biggest_height = cell->height;
				add_to_list = 1;
			}

			if (add_to_list)
			{
				if (r->side == 0)
					r->perp_wall_dist = (side_dist.x - delta_dist.x);
				else
					r->perp_wall_dist = (side_dist.y - delta_dist.y);
				if (r->perp_wall_dist > m->dof)
					break ;
				list_add_front(list, list_new(r));
				r = calloc(1, sizeof(t_ray));
				r->ray_dir = ((t_ray *)(*list)->data)->ray_dir;
				r->m_index = ((t_ray *)(*list)->data)->m_index;
				r->side = ((t_ray *)(*list)->data)->side;
			}
		}
	}
	free(r);
	return ;
}

static int	is_obstacle_see_through(t_map *m, t_ray *r)
{
	t_img	*tex;
	t_cell	*cell;

	tex = 0;
	cell = &m->cells[r->m_index.y * m->size.x + r->m_index.x];
	if (r->side == 1 && r->ray_dir.y > 0)
		tex = cell->tex_north;
	else if (r->side == 1 && r->ray_dir.y < 0)
		tex = cell->tex_south;
	else if (r->side == 0 && r->ray_dir.x > 0)
		tex = cell->tex_west;
	else if (r->side == 0 && r->ray_dir.x < 0)
		tex = cell->tex_east;
	if (!tex)
		return (1);
	return (tex->is_see_through[tex->cycle_index]);
}
