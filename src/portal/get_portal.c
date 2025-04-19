#include "cimmerian.h"

static void	get_face_line(char face, t_ivec2 cell, double offset, t_vec2 *line);

/* Trigger portal only if portal face is touched, not just the cell */
t_portal	*get_portal(t_man *man)
{
	t_map		*map;
	t_portal	*portal;
	t_vec2		face_line[2];

	map = man->maps[man->curr_map];
	if (!map->cells)
		return (0);
	portal = map->cells[(int)man->player.pos.y][(int)man->player.pos.x].portal;
	if (!portal)
		return (0);
	if (!man->player.is_in_portal)
		get_face_line(portal->src_cardinal, portal->src_pos, 0, face_line);
	else if (portal->is_corridor)
		get_face_line(opp_cardinal(portal->src_cardinal), portal->src_pos, 0.01,
			face_line);
	else
		return (0);
	if (do_segments_intersect(face_line[0], face_line[1], man->player.prev_pos,
			man->player.pos))
	{
		portal->trigger_opposite = man->player.is_in_portal;
		return (portal);
	}
	return (0);
}

static void	get_face_line(char face, t_ivec2 cell, double offset, t_vec2 *line)
{
	t_vec2	dir;
	t_vec2	p0;
	t_vec2	p1;
	t_vec2	p2;

	dir = get_dir_from_cardinal(face);
	set_vec2(&p0, cell.x, cell.y);
	set_vec2(&p1, cell.x + 1, cell.y + 1);
	if (dir.x)
	{
		p2.x = p0.x + offset;
		if (dir.x > 0)
			p2.x = p1.x - offset;
		set_vec2(line + 0, p2.x, p0.y);
		set_vec2(line + 1, p2.x, p1.y);
	}
	else
	{
		p2.y = p0.y + offset;
		if (dir.y > 0)
			p2.y = p1.y - offset;
		set_vec2(line + 0, p0.x, p2.y);
		set_vec2(line + 1, p1.x, p2.y);
	}
	return ;
}
