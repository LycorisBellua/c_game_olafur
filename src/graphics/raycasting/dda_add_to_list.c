#include "cimmerian.h"

static void	set_perp_wall_dist(t_man *man, t_ray **r, t_cell *cell,
				int *add_to_list);
static int	set_perp_wall_dist_with_offset(t_man *man, t_ray *r, double offset);
static void	add_ray_to_list(t_man *man, t_ray **r);

int	dda_add_to_list(t_man *man, t_map *m, t_ray **r, double *max_height)
{
	t_cell	*cell;
	int		add_to_list;

	if ((*r)->m_index.x < 0 || (*r)->m_index.x >= m->size.x
		|| (*r)->m_index.y < 0 || (*r)->m_index.y >= m->size.y)
		return (-1);
	cell = &m->cells[(*r)->m_index.y][(*r)->m_index.x];
	if (cell->portal)
		add_map(man, cell->portal->path_map);
	if (!cell->is_visible)
		return (0);
	add_to_list = 0;
	set_texture_and_is_see_through(*r, cell);
	if ((*r)->is_see_through || cell->is_door)
		add_to_list = 1;
	else if (cell->height > *max_height)
	{
		*max_height = cell->height;
		add_to_list = 1;
	}
	set_perp_wall_dist(man, r, cell, &add_to_list);
	if (add_to_list > 0)
		add_ray_to_list(man, r);
	return (add_to_list);
}

static void	set_perp_wall_dist(t_man *man, t_ray **r, t_cell *cell,
	int *add_to_list)
{
	if (!*add_to_list)
		return ;
	else if (cell->is_door)
		*add_to_list = set_perp_wall_dist_with_offset(man, *r, 0.5);
	else
	{
		if ((*r)->side == 0)
			(*r)->perp_wall_dist = (*r)->side_dist.x - (*r)->delta_dist.x;
		else
			(*r)->perp_wall_dist = (*r)->side_dist.y - (*r)->delta_dist.y;
		if ((*r)->perp_wall_dist > man->dof)
			*add_to_list = -1;
	}
	return ;
}

static int	set_perp_wall_dist_with_offset(t_man *man, t_ray *r, double offset)
{
	t_vec2	wall;

	if (r->side == 0)
	{
		wall.x = r->m_index.x + offset;
		wall.y = man->player.pos.y + (wall.x - man->player.pos.x)
			* (r->ray_dir.y / r->ray_dir.x);
		if (wall.y < r->m_index.y || wall.y > r->m_index.y + 1)
			return (0);
		r->perp_wall_dist = (wall.x - man->player.pos.x) / r->ray_dir.x;
	}
	else
	{
		wall.y = r->m_index.y + offset;
		wall.x = man->player.pos.x + (wall.y - man->player.pos.y)
			* (r->ray_dir.x / r->ray_dir.y);
		if (wall.x < r->m_index.x || wall.x > r->m_index.x + 1)
			return (0);
		r->perp_wall_dist = (wall.y - man->player.pos.y) / r->ray_dir.y;
	}
	if (r->perp_wall_dist > man->dof)
		return (-1);
	return (1);
}

static void	add_ray_to_list(t_man *man, t_ray **r)
{
	list_add_front(&man->rays, list_new(*r));
	*r = calloc(1, sizeof(t_ray));
	if (*r)
		memcpy(*r, ((t_ray *)man->rays->data), sizeof(t_ray));
	return ;
}
