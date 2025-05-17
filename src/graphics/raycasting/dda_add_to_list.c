#include "cimmerian.h"

static int	is_ray_hitting_portal(t_cell *cell, t_ray *r);
static void	set_perp_wall_dist(t_man *man, t_ray **r, t_cell *cell,
				int *add_to_list);
static int	set_perp_wall_dist_with_offset(t_man *man, t_ray *r, double offset);
static void	add_ray_to_list(t_man *man, t_ray **r);

t_vec2	get_hit_point(t_ray *r)
{
	t_vec2	hit_point;
	double	t;

	// Ray hits a horizontal wall: y is fixed
	if (r->side == 1) // horizontal wall (North or South)
	{
		double wall_y = r->m_index.y + (r->step.y < 0 ? 1.0 : 0.0); // North: top edge, South: bottom edge
		t = (wall_y - r->origin_pos.y) / r->ray_dir.y;
		hit_point.x = r->origin_pos.x + t * r->ray_dir.x;
		hit_point.y = wall_y;
	}
	else // vertical wall (East or West)
	{
		double wall_x = r->m_index.x + (r->step.x < 0 ? 1.0 : 0.0); // West: right edge, East: left edge
		t = (wall_x - r->origin_pos.x) / r->ray_dir.x;
		hit_point.x = wall_x;
		hit_point.y = r->origin_pos.y + t * r->ray_dir.y;
	}
	return (hit_point);
}

t_vec2	project_hit(t_vec2 old_hit, t_ivec2 old_cell, char old_face,
	t_ivec2 new_cell, char new_face)
{
	double	offset;
	t_vec2	new_hit;

	if (old_face == 'N' || old_face == 'S')
		offset = old_hit.x - old_cell.x;
	else
		offset = old_hit.y - old_cell.y;

	if (new_face == 'N')
	{
		new_hit.y = new_cell.y;
		new_hit.x = new_cell.x + offset;
	}
	else if (new_face == 'S')
	{
		new_hit.y = new_cell.y + 1.0;
		new_hit.x = new_cell.x + offset;
	}
	else if (new_face == 'W')
	{
		new_hit.x = new_cell.x;
		new_hit.y = new_cell.y + offset;
	}
	else if (new_face == 'E')
	{
		new_hit.x = new_cell.x + 1.0;
		new_hit.y = new_cell.y + offset;
	}

	return (new_hit);
}

void transform_portal_ray(t_ray *r, const t_cell *cell, t_vec2 src_hit, t_vec2 dst_hit)
{
    // Step 1: Offset from player's position to source hit
    t_vec2 offset = {
        r->origin_pos.x - src_hit.x,
        r->origin_pos.y - src_hit.y
    };

    // Step 2: Determine source/destination face normals
    t_vec2 src_norm = get_dir_from_cardinal(cell->portal->src_cardinal);
    t_vec2 dst_norm = get_dir_from_cardinal(opp_cardinal(cell->portal->dst_cardinal));

    // Step 3: Compute rotation angle between the normals
    double angle = atan2(dst_norm.y, dst_norm.x) - atan2(src_norm.y, src_norm.x);

    // Step 4: Rotate offset, direction, and plane
    t_vec2 rotated_offset = rotate_vec2(offset, angle);
    t_vec2 rotated_dir    = rotate_vec2(r->origin_dir, angle);
    t_vec2 rotated_plane  = rotate_vec2(r->origin_plane, angle);

    // Step 5: Update ray data
    r->origin_pos.x = dst_hit.x + rotated_offset.x;
    r->origin_pos.y = dst_hit.y + rotated_offset.y;
    r->origin_dir   = rotated_dir;
    r->origin_plane = rotated_plane;
	return ;
}

int	dda_add_to_list(t_man *man, t_ray **r, double *max_height)
{
	t_cell	*cell;
	int		add_to_list;

	if ((*r)->m_index.x < 0 || (*r)->m_index.x >= (*r)->m->size.x
		|| (*r)->m_index.y < 0 || (*r)->m_index.y >= (*r)->m->size.y)
		return (-1);
	cell = &(*r)->m->cells[(*r)->m_index.y][(*r)->m_index.x];
	if (is_ray_hitting_portal(cell, *r))
	{
		/**/
		int	map_index = add_map(man, cell->portal->path_map);
		if (map_index < 0)
			return (-1);

		t_vec2	src_hit = get_hit_point(*r);
		t_vec2	dst_hit = project_hit(src_hit, (*r)->m_index,
			cell->portal->src_cardinal, cell->portal->dst_pos,
			opp_cardinal(cell->portal->dst_cardinal));

		double src_perp_wall_dist;
		if ((*r)->side == 0)
			src_perp_wall_dist = (*r)->side_dist.x - (*r)->delta_dist.x;
		else
			src_perp_wall_dist = (*r)->side_dist.y - (*r)->delta_dist.y;

		transform_portal_ray(*r, cell, src_hit, dst_hit);
		(*r)->m = man->maps[map_index];


		// REDO RAYCASTING
		init_ray_data_x(*r);
		init_ray_data_y(*r);
		(*r)->perp_wall_dist = 0;
		int iterations = 0;
		while (iterations < 50 && (*r)->perp_wall_dist < src_perp_wall_dist)
		{
			update_ray_data(*r);
			if ((*r)->side == 0)
				(*r)->perp_wall_dist = (*r)->side_dist.x - (*r)->delta_dist.x;
			else
				(*r)->perp_wall_dist = (*r)->side_dist.y - (*r)->delta_dist.y;
			++iterations;
		}
		if ((*r)->m_index.x < 0 || (*r)->m_index.x >= (*r)->m->size.x
			|| (*r)->m_index.y < 0 || (*r)->m_index.y >= (*r)->m->size.y)
			return (-1);
		cell = &(*r)->m->cells[(*r)->m_index.y][(*r)->m_index.x];
		/**/
	}
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

static int	is_ray_hitting_portal(t_cell *cell, t_ray *r)
{
	if (!cell->portal)
		return (0);
	else if (r->side == 1 && r->ray_dir.y > 0)
		return (cell->portal->src_cardinal == 'N');
	else if (r->side == 1 && r->ray_dir.y < 0)
		return (cell->portal->src_cardinal == 'S');
	else if (r->side == 0 && r->ray_dir.x > 0)
		return (cell->portal->src_cardinal == 'W');
	else if (r->side == 0 && r->ray_dir.x < 0)
		return (cell->portal->src_cardinal == 'E');
	return (0);
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
