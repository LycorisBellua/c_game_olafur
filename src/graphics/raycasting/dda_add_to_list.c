#include "cimmerian.h"

static int	is_cell_see_through(t_map *m, t_ray *r);
static void	set_perp_wall_dist(t_man *man, t_ray *r, int *add_to_list);

int	dda_add_to_list(t_man *man, t_map *m, t_ray *r, double *max_height)
{
	t_cell	*cell;
	int		add_to_list;

	if (r->m_index.x < 0 || r->m_index.x >= m->size.x
		|| r->m_index.y < 0 || r->m_index.y >= m->size.y)
		return (-1);
	add_to_list = 0;
	cell = &m->cells[r->m_index.y][r->m_index.x];
	if (cell->portal)
		add_map(man, cell->portal->path_map);
	if (cell->is_visible)
	{
		r->is_see_through = is_cell_see_through(m, r);
		if (r->is_see_through)
			add_to_list = 1;
		else if (cell->height > *max_height)
		{
			*max_height = cell->height;
			add_to_list = 1;
		}
		if (add_to_list)
			set_perp_wall_dist(man, r, &add_to_list);
	}
	return (add_to_list);
}

static int	is_cell_see_through(t_map *m, t_ray *r)
{
	t_img	*tex;
	t_cell	*cell;

	tex = 0;
	cell = &m->cells[r->m_index.y][r->m_index.x];
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

static void	set_perp_wall_dist(t_man *man, t_ray *r, int *add_to_list)
{
	if (r->side == 0)
		r->perp_wall_dist = (r->side_dist.x - r->delta_dist.x);
	else
		r->perp_wall_dist = (r->side_dist.y - r->delta_dist.y);
	if (r->perp_wall_dist > man->dof)
		*add_to_list = -1;
	return ;
}
