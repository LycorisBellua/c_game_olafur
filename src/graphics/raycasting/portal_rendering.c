#include "cimmerian.h"

static int		is_within_portal_and_ray_hits_opp_face(t_ray *r);
static int		is_ray_hitting_portal(t_cell *cell, t_ray *r);
static t_vec2	get_hit_point(t_ray *r);
static t_vec2	project_hit(t_vec2 src_hit, t_ivec2 src_cell, char src_face,
					t_ivec2 dst_cell, char dst_face);
static int		should_flip_offset(char src_face, char dst_face);
static void		transform_portal_ray(t_ray *r, t_vec2 src_hit, t_vec2 dst_hit,
					char src_face, char dst_face);

t_cell	*resolve_portal_rendering(t_man *man, t_ray **r)
{
	t_cell	*cell;
	int		internal_hit;
	int		external_hit;
	int		map_index;
	char	src_face;
	char	dst_face;
	t_vec2	src_hit;
	t_vec2	dst_hit;
	double	src_perp_wall_dist;

	/*
		- In the case of map_0 having two portals on both ends of the map, the 
		northern portal face glitches. It shouldn't display the wall of the 
		destination portal, it should look like a corridor instead.
		- Also, with these two portals, sometimes the game freezes (infinite 
		loop?) or suddenly drops to >10 FPS because going back to 100.

		- Sometimes, the (6, 5) portal of map_1 "teleports" us a little bit 
		when crossed. It's not always the case but when it happens it's jarring.
		- When looking into map_1 from map_0, the tower of map_1 can be seen 
		but only through the portal face. I don't know whether to leave it this 
		way, or make it so the map_1 tower can be seen even outside of the 
		portal, beyond the northern wall of map_0. Or even, I don't know 
		whether to make the portal face an actual portal, and therefore it cuts 
		off whatever pixels don't belong to the src portal face (or, maybe this 
		becomes the case only when the portal is of teleporter-type, and not a 
		corridor-type).
		- When the portal is of teleporter-type, should the rendering starts 
		from the opposite dst face like it currently does, or should it start 
		from the actual dst face?
		- That would be great if, instead of redoing the whole raycasting, the 
		current ray can just be fixed to match the new origin. Is it easy?

		- Once the walls are perfect, consider doing the rest in this order: 
		the ceiling, the floor, and the sprites. For example, when there's a 
		portal around the 6 little cubes in map_0, and you look at it from 
		within map_1, you can see the tower of "map_1" meshing with the white 
		pillars. The pillars should be occluded, because there's a portal face 
		in front of them.
	*/

	if ((*r)->m_index.x < 0 || (*r)->m_index.x >= (*r)->m->size.x
		|| (*r)->m_index.y < 0 || (*r)->m_index.y >= (*r)->m->size.y)
		cell = 0;
	else
		cell = &(*r)->m->cells[(*r)->m_index.y][(*r)->m_index.x];
	/**/return (cell);
	internal_hit = is_within_portal_and_ray_hits_opp_face(*r);
	external_hit = is_ray_hitting_portal(cell, *r);
	if (!internal_hit && !external_hit)
		return (cell);
	else if (!cell && internal_hit)
		cell = &(*r)->m->cells[(int)(*r)->origin_pos.y][(int)(*r)->origin_pos.x];

	// I shouldn't need to check, but I do, otherwise it crashes when I cross 
	// the portal around the circle of six cubes through a non-portal face.
	// Also, only raycast through the portal if `portal->preview` is true.
	if (!cell->portal)
		return (cell);
	//
	map_index = add_map(man, cell->portal->path_dst_map);
	if (map_index < 0)
		return (0);

	if (internal_hit)
	{
		src_face = opp_cardinal(cell->portal->src_cardinal);
		dst_face = cell->portal->dst_cardinal;
	}
	else
	{
		src_face = cell->portal->src_cardinal;
		dst_face = opp_cardinal(cell->portal->dst_cardinal);
	}

	src_hit = get_hit_point(*r);
	dst_hit = project_hit(src_hit, (*r)->m_index, src_face,
		cell->portal->dst_pos, dst_face);

	if ((*r)->side == 0)
		src_perp_wall_dist = (*r)->side_dist.x - (*r)->delta_dist.x;
	else
		src_perp_wall_dist = (*r)->side_dist.y - (*r)->delta_dist.y;

	transform_portal_ray(*r, src_hit, dst_hit, src_face, dst_face);
	(*r)->m = man->maps[map_index];

	// REDO RAYCASTING ---
	init_ray_data_x(*r);
	init_ray_data_y(*r);
	(*r)->perp_wall_dist = 0;
	while ((*r)->perp_wall_dist < src_perp_wall_dist)
	{
		update_ray_data(*r);
		if ((*r)->side == 0)
			(*r)->perp_wall_dist = (*r)->side_dist.x - (*r)->delta_dist.x;
		else
			(*r)->perp_wall_dist = (*r)->side_dist.y - (*r)->delta_dist.y;
	}
	// ---
	if ((*r)->m_index.x < 0 || (*r)->m_index.x >= (*r)->m->size.x
			|| (*r)->m_index.y < 0 || (*r)->m_index.y >= (*r)->m->size.y)
		return (0);
	cell = &(*r)->m->cells[(*r)->m_index.y][(*r)->m_index.x];
	return (cell);
}

static int	is_within_portal_and_ray_hits_opp_face(t_ray *r)
{
	t_ivec2		pos;
	t_portal	*portal;

	set_ivec2(&pos, r->origin_pos.x, r->origin_pos.y);
	portal = 0;
	if (!(pos.x < 0 || pos.x >= r->m->size.x || pos.y < 0
				|| pos.y >= r->m->size.y))
		portal = r->m->cells[pos.y][pos.x].portal;
	if (!portal)
		return (0);
	else if (r->side == 1 && r->ray_dir.y > 0)
		return (portal->src_cardinal == 'N');
	else if (r->side == 1 && r->ray_dir.y < 0)
		return (portal->src_cardinal == 'S');
	else if (r->side == 0 && r->ray_dir.x > 0)
		return (portal->src_cardinal == 'W');
	else if (r->side == 0 && r->ray_dir.x < 0)
		return (portal->src_cardinal == 'E');
	return (0);
}

static int	is_ray_hitting_portal(t_cell *cell, t_ray *r)
{
	if (!cell || !cell->portal)
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

static t_vec2	get_hit_point(t_ray *r)
{
	t_vec2	hit_point;
	t_vec2	wall;
	double	t;

	if (r->side == 1)
	{
		wall.y = r->m_index.y + (r->step.y < 0) * 1.0;
		t = (wall.y - r->origin_pos.y) / r->ray_dir.y;
		hit_point.x = r->origin_pos.x + t * r->ray_dir.x;
		hit_point.y = wall.y;
	}
	else
	{
		wall.x = r->m_index.x + (r->step.x < 0) * 1.0;
		t = (wall.x - r->origin_pos.x) / r->ray_dir.x;
		hit_point.x = wall.x;
		hit_point.y = r->origin_pos.y + t * r->ray_dir.y;
	}
	return (hit_point);
}

static t_vec2	project_hit(t_vec2 src_hit, t_ivec2 src_cell, char src_face,
		t_ivec2 dst_cell, char dst_face)
{
	double	offset;
	t_vec2	dst_hit;

	if (src_face == 'N' || src_face == 'S')
		offset = src_hit.x - src_cell.x;
	else
		offset = src_hit.y - src_cell.y;
	if (should_flip_offset(src_face, dst_face))
		offset = 1.0 - offset;
	if (dst_face == 'N')
		set_vec2(&dst_hit, dst_cell.x + offset, dst_cell.y);
	else if (dst_face == 'S')
		set_vec2(&dst_hit, dst_cell.x + offset, dst_cell.y + 1.0);
	else if (dst_face == 'W')
		set_vec2(&dst_hit, dst_cell.x, dst_cell.y + offset);
	else if (dst_face == 'E')
		set_vec2(&dst_hit, dst_cell.x + 1.0, dst_cell.y + offset);
	return (dst_hit);
}

static int	should_flip_offset(char src_face, char dst_face)
{
	t_ivec2	src_dir;
	t_ivec2	dst_dir;

	if (src_face == dst_face)
		return (0);
	src_dir = get_dir_from_cardinal_ivec(src_face);
	dst_dir = get_dir_from_cardinal_ivec(dst_face);
	return ((src_dir.x + src_dir.y) == (dst_dir.x + dst_dir.y));
}

static void	transform_portal_ray(t_ray *r, t_vec2 src_hit, t_vec2 dst_hit,
		char src_face, char dst_face)
{
	t_vec2	offset;
	t_vec2	src_norm;
	t_vec2	dst_norm;
	double	angle;
	t_vec2	rotated_offset;
	t_vec2	rotated_dir;
	t_vec2	rotated_plane;

	// Offset from player's position to source hit
	set_vec2(&offset, r->origin_pos.x - src_hit.x, r->origin_pos.y - src_hit.y);

	// Determine source/destination face normals
	src_norm = get_dir_from_cardinal(src_face);
	dst_norm = get_dir_from_cardinal(dst_face);

	// Compute rotation angle between the normals
	angle = atan2(dst_norm.y, dst_norm.x) - atan2(src_norm.y, src_norm.x);

	// Rotate offset, direction, and plane
	rotated_offset = rotate_vec2(offset, angle);
	rotated_dir = rotate_vec2(r->origin_dir, angle);
	rotated_plane = rotate_vec2(r->origin_plane, angle);

	// Update ray data
	r->origin_pos.x = dst_hit.x + rotated_offset.x;
	r->origin_pos.y = dst_hit.y + rotated_offset.y;
	r->origin_dir = rotated_dir;
	r->origin_plane = rotated_plane;
	return ;
}
