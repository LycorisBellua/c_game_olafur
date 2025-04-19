#include "cimmerian.h"

static void	init_ray_data_x(t_man *man, t_ray *r, double cam_x);
static void	init_ray_data_y(t_man *man, t_ray *r, double cam_x);
static void	update_ray_data(t_ray *r);

void	perform_dda(t_man *man, double cam_x, t_list **list)
{
	t_ray	*r;
	double	max_height;
	int		add;

	r = calloc(1, sizeof(t_ray));
	if (!r)
		return ;
	init_ray_data_x(man, r, cam_x);
	init_ray_data_y(man, r, cam_x);
	max_height = 0;
	while (1)
	{
		update_ray_data(r);
		add = dda_add_to_list(man, man->maps[man->curr_map], r, &max_height);
		if (add < 0)
			break ;
		else if (add)
		{
			list_add_front(list, list_new(r));
			r = calloc(1, sizeof(t_ray));
			memcpy(r, ((t_ray *)(*list)->data), sizeof(t_ray));
		}
	}
	free(r);
	return ;
}

static void	init_ray_data_x(t_man *man, t_ray *r, double cam_x)
{
	r->ray_dir.x = man->player.dir.x + man->player.plane.x * cam_x;
	r->delta_dist.x = HUGE_VAL;
	if (r->ray_dir.x)
		r->delta_dist.x = fabs(1 / r->ray_dir.x);
	r->m_index.x = (int)man->player.pos.x;
	if (r->ray_dir.x < 0)
	{
		r->step.x = -1;
		r->side_dist.x = (man->player.pos.x - r->m_index.x) * r->delta_dist.x;
	}
	else
	{
		r->step.x = 1;
		r->side_dist.x = (r->m_index.x + 1.0 - man->player.pos.x)
			* r->delta_dist.x;
	}
	return ;
}

static void	init_ray_data_y(t_man *man, t_ray *r, double cam_x)
{
	r->ray_dir.y = man->player.dir.y + man->player.plane.y * cam_x;
	r->delta_dist.y = HUGE_VAL;
	if (r->ray_dir.y)
		r->delta_dist.y = fabs(1 / r->ray_dir.y);
	r->m_index.y = (int)man->player.pos.y;
	if (r->ray_dir.y < 0)
	{
		r->step.y = -1;
		r->side_dist.y = (man->player.pos.y - r->m_index.y) * r->delta_dist.y;
	}
	else
	{
		r->step.y = 1;
		r->side_dist.y = (r->m_index.y + 1.0 - man->player.pos.y)
			* r->delta_dist.y;
	}
	return ;
}

static void	update_ray_data(t_ray *r)
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
