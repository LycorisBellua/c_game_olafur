#include "olafur.h"

static void	set_perp_wall_dist(t_man *man, t_ray **r, t_cell *cell,
				int *add_to_list);
static int	get_door_plane(t_door *d, float *offset);
static int	set_perp_wall_dist_with_offset(t_man *man, t_ray *r, float offset,
				int axis);
static void	add_ray_to_list(t_man *man, t_ray **r);

int	dda_add_to_list(t_man *man, t_ray **r, float *max_height)
{
	t_cell	*cell;
	int		add_to_list;

	cell = get_cell((*r)->m, (*r)->m_index.x, (*r)->m_index.y);
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
	int		axis;

	if (!*add_to_list)
		return ;
	else if (cell->door && cell->door->is_plane)
	{
		axis = get_door_plane(cell->door, &offset);
		if (axis < 0)
			*add_to_list = 0;
		else
			*add_to_list = set_perp_wall_dist_with_offset(man, *r, offset,
					axis);
	}
	else
	{
		if ((*r)->side == 0)
			(*r)->perp_wall_dist = (*r)->side_dist.x - (*r)->delta_dist.x;
		else
			(*r)->perp_wall_dist = (*r)->side_dist.y - (*r)->delta_dist.y;
		if ((*r)->perp_wall_dist * (*r)->ray_len > man->dof)
			*add_to_list = -1;
	}
	return ;
}

/*
	Returns the axis the door's plane is perpendicular to, like `side`: 1 for a
	plane running from west to east (at `offset` along y), 0 for one running
	from north to south (at `offset` along x), or -1 if there's no plane.
*/
static int	get_door_plane(t_door *d, float *offset)
{
	*offset = 0.5f;
	if (d->offset == 'N' || d->offset == 'W')
		*offset = 0.0f;
	else if (d->offset == 'S' || d->offset == 'E')
		*offset = 1.0f;
	if (d->offset == 'H' || d->offset == 'N' || d->offset == 'S')
		return (1);
	else if (d->offset == 'V' || d->offset == 'W' || d->offset == 'E')
		return (0);
	return (-1);
}

/*
	The plane is hit wherever the ray crosses it within the cell, whichever side
	the ray entered the cell from. `side` then becomes the plane's, so that the
	door is textured as a face of that orientation. A ray parallel to the
	plane gives an infinite or NaN coordinate, which the range check rejects.
*/
static int	set_perp_wall_dist_with_offset(t_man *man, t_ray *r, float offset,
	int axis)
{
	t_vec2	wall;

	if (axis == 0)
	{
		wall.x = r->m_index.x + offset;
		wall.y = r->origin_pos.y + (wall.x - r->origin_pos.x)
			* (r->ray_dir.y / r->ray_dir.x);
		if (!(wall.y >= r->m_index.y && wall.y <= r->m_index.y + 1))
			return (0);
		r->perp_wall_dist = (wall.x - r->origin_pos.x) / r->ray_dir.x;
	}
	else
	{
		wall.y = r->m_index.y + offset;
		wall.x = r->origin_pos.x + (wall.y - r->origin_pos.y)
			* (r->ray_dir.x / r->ray_dir.y);
		if (!(wall.x >= r->m_index.x && wall.x <= r->m_index.x + 1))
			return (0);
		r->perp_wall_dist = (wall.y - r->origin_pos.y) / r->ray_dir.y;
	}
	r->side = axis;
	if (r->perp_wall_dist * r->ray_len > man->dof)
		return (-1);
	return (1);
}

static void	add_ray_to_list(t_man *man, t_ray **r)
{
	if (man->ray_count < MAX_COLUMN_RAYS)
		man->ray_buf[man->ray_count++] = **r;
	return ;
}
