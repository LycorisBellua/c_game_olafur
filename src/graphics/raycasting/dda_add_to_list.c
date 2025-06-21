#include "olafur.h"

static void	set_perp_wall_dist(t_man *man, t_ray **r, t_cell *cell,
				int *add_to_list);
static int	get_door_plane_offset(t_door *d, t_ray *r, float *offset);
static int	set_perp_wall_dist_with_offset(t_man *man, t_ray *r, float offset);
static void	add_ray_to_list(t_man *man, t_ray **r);

int	dda_add_to_list(t_man *man, t_ray **r, float *max_height)
{
	t_cell	*cell;
	int		add_to_list;

	cell = get_cell((*r)->m, (*r)->m_index.x, (*r)->m_index.y);
	cell = resolve_portal_rendering(man, r, cell);
	if (!cell)
		return (-1);
	else if (!cell->is_visible)
		return (0);
	add_to_list = 0;
	set_texture_and_is_see_through(*r, cell);
	if ((*r)->is_see_through || (cell->door && cell->door->is_plane))
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
	float	offset;

	if (!*add_to_list)
		return ;
	else if (cell->door && cell->door->is_plane)
	{
		if (!get_door_plane_offset(cell->door, *r, &offset))
			*add_to_list = 0;
		else
			*add_to_list = set_perp_wall_dist_with_offset(man, *r, offset);
	}
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

static int	get_door_plane_offset(t_door *d, t_ray *r, float *offset)
{
	if (d->offset == 'H' || d->offset == 'N' || d->offset == 'S')
	{
		if (r->side == 0)
			return (0);
		else if (d->offset == 'H')
			*offset = 0.5;
		else if (d->offset == 'N')
			*offset = 0.0;
		else if (d->offset == 'S')
			*offset = 1.0;
		return (1);
	}
	else if (d->offset == 'V' || d->offset == 'W' || d->offset == 'E')
	{
		if (r->side == 1)
			return (0);
		else if (d->offset == 'V')
			*offset = 0.5;
		else if (d->offset == 'W')
			*offset = 0.0;
		else if (d->offset == 'E')
			*offset = 1.0;
		return (1);
	}
	return (0);
}

static int	set_perp_wall_dist_with_offset(t_man *man, t_ray *r, float offset)
{
	t_vec2	wall;

	if (r->side == 0)
	{
		wall.x = r->m_index.x + offset;
		wall.y = r->origin_pos.y + (wall.x - r->origin_pos.x)
			* (r->ray_dir.y / r->ray_dir.x);
		if (wall.y < r->m_index.y || wall.y > r->m_index.y + 1)
			return (0);
		r->perp_wall_dist = (wall.x - r->origin_pos.x) / r->ray_dir.x;
	}
	else
	{
		wall.y = r->m_index.y + offset;
		wall.x = r->origin_pos.x + (wall.y - r->origin_pos.y)
			* (r->ray_dir.x / r->ray_dir.y);
		if (wall.x < r->m_index.x || wall.x > r->m_index.x + 1)
			return (0);
		r->perp_wall_dist = (wall.y - r->origin_pos.y) / r->ray_dir.y;
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
