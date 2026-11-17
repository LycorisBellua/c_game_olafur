#include "olafur.h"

static int		is_colliding(t_man *man, t_cell *cell, char opposite_cardinal);
static int		is_in_available_cell(int collision[4]);
static void		push_out_of_corners(t_man *man, t_map *m);

int	push_back_on_collision(t_man *man, t_map *m, t_vec2 pos, t_vec2 delta)
{
	int	collision[4];

	collision[0] = is_colliding(man, get_cell(m, pos.x,
		pos.y + man->player.radius), 'S');
	if (collision[0] && delta.y > 0)
		pos.y = floorf(pos.y + man->player.radius) - man->player.radius;
	collision[1] = is_colliding(man, get_cell(m, pos.x,
		pos.y - man->player.radius), 'N');
	if (collision[1] && delta.y < 0)
		pos.y = ceilf(pos.y - man->player.radius) + man->player.radius;
	collision[2] = is_colliding(man, get_cell(m, pos.x + man->player.radius,
		pos.y), 'E');
	if (collision[2] && delta.x > 0)
		pos.x = floorf(pos.x + man->player.radius) - man->player.radius;
	collision[3] = is_colliding(man, get_cell(m, pos.x - man->player.radius,
		pos.y), 'W');
	if (collision[3] && delta.x < 0)
		pos.x = ceilf(pos.x - man->player.radius) + man->player.radius;
	man->player.pos = pos;
	push_out_of_corners(man, m);
	prevent_out_of_bounds(man);
	return (is_in_available_cell(collision));
}

t_cell	*get_cell(t_map *m, int x, int y)
{
	if (x < 0 || y < 0 || x >= m->size.x || y >= m->size.y)
		return (0);
	return (&m->cells[y][x]);
}

static int	is_colliding(t_man *man, t_cell *cell, char opposite_cardinal)
{
	char	portal_face;

	portal_face = 0;
	if (!man->player.is_in_portal && cell && cell->portal)
		portal_face = cell->portal->src_cardinal;
	return (!cell || (cell->is_obstacle
		&& (!portal_face || portal_face == opposite_cardinal)
		&& (!man->player.is_in_portal || !cell->portal)));
}

static int	is_in_available_cell(int collision[4])
{
	int	i;
	int	nbr_collisions;

	i = 0;
	nbr_collisions = 0;
	while (i < 4)
	{
		nbr_collisions += collision[i];
		++i;
	}
	return (nbr_collisions < 3);
}

/*
	The checks above only look straight up, down, left and right of the
	player, so the corner of a diagonal wall could get inside the player, who
	would then end up within the wall when sliding off of it. The player is
	pushed away from such a corner instead. Only outer corners can be this
	close, since the faces of a neighboring wall are already kept at bay.
*/
static void	push_out_of_corners(t_man *man, t_map *m)
{
	t_vec2	p;
	t_vec2	corner;
	t_cell	*cell;
	float	d;
	int		i;

	i = -1;
	while (++i < 4)
	{
		p = man->player.pos;
		set_vec2(&corner, (int)p.x + i % 2, (int)p.y + i / 2);
		d = dist(p, corner);
		cell = get_cell(m, (int)p.x + i % 2 * 2 - 1, (int)p.y + i / 2 * 2 - 1);
		if (d >= man->player.radius || d <= EPSILON || (cell
				&& (!cell->is_obstacle
					|| (man->player.is_in_portal && cell->portal))))
			continue ;
		set_vec2(&man->player.pos,
			corner.x + (p.x - corner.x) / d * man->player.radius,
			corner.y + (p.y - corner.y) / d * man->player.radius);
	}
	return ;
}
