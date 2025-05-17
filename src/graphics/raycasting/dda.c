#include "cimmerian.h"

void	perform_dda(t_man *man, double cam_x)
{
	t_ray	*r;
	double	max_height;

	r = calloc(1, sizeof(t_ray));
	if (!r)
		return ;
	r->cam_x = cam_x;
	r->m = man->maps[man->curr_map];
	r->origin_pos = man->player.pos;
	r->origin_dir = man->player.dir;
	r->origin_plane = man->player.plane;
	init_ray_data_x(r);
	init_ray_data_y(r);
	max_height = 0;
	while (1)
	{
		update_ray_data(r);
		if (dda_add_to_list(man, &r, &max_height) < 0)
			break ;
	}
	free(r);
	return ;
}

void	init_ray_data_x(t_ray *r)
{
	r->ray_dir.x = r->origin_dir.x + r->origin_plane.x * r->cam_x;
	r->delta_dist.x = HUGE_VAL;
	if (r->ray_dir.x)
		r->delta_dist.x = fabs(1 / r->ray_dir.x);
	r->m_index.x = (int)r->origin_pos.x;
	if (r->ray_dir.x < 0)
	{
		r->step.x = -1;
		r->side_dist.x = (r->origin_pos.x - r->m_index.x) * r->delta_dist.x;
	}
	else
	{
		r->step.x = 1;
		r->side_dist.x = (r->m_index.x + 1.0 - r->origin_pos.x)
			* r->delta_dist.x;
	}
	return ;
}

void	init_ray_data_y(t_ray *r)
{
	r->ray_dir.y = r->origin_dir.y + r->origin_plane.y * r->cam_x;
	r->delta_dist.y = HUGE_VAL;
	if (r->ray_dir.y)
		r->delta_dist.y = fabs(1 / r->ray_dir.y);
	r->m_index.y = (int)r->origin_pos.y;
	if (r->ray_dir.y < 0)
	{
		r->step.y = -1;
		r->side_dist.y = (r->origin_pos.y - r->m_index.y) * r->delta_dist.y;
	}
	else
	{
		r->step.y = 1;
		r->side_dist.y = (r->m_index.y + 1.0 - r->origin_pos.y)
			* r->delta_dist.y;
	}
	return ;
}

void	update_ray_data(t_ray *r)
{
	if (r->side_dist.x < r->side_dist.y)
	{
		r->side_dist.x += r->delta_dist.x;
		r->m_index.x += r->step.x;
		r->side = 0;
	}
	else
	{
		r->side_dist.y += r->delta_dist.y;
		r->m_index.y += r->step.y;
		r->side = 1;
	}
	return ;
}
