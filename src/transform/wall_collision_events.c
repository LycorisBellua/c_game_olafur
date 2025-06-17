#include "olafur.h"

void	cross_goal_if_unlocked(t_man *man, t_map *m)
{
	t_vec2	pos;
	double	radius;
	t_cell	*cell[4];

	pos = man->player.pos;
	radius = man->player.radius;
	bzero(&cell, 4 * sizeof(t_cell *));
	if ((int)(pos.y + radius) < m->size.y)
		cell[0] = &m->cells[(int)(pos.y + radius)][(int)pos.x];
	if ((int)(pos.y - radius) >= 0)
		cell[1] = &m->cells[(int)(pos.y - radius)][(int)pos.x];
	if ((int)(pos.x + radius) < m->size.x)
		cell[2] = &m->cells[(int)pos.y][(int)(pos.x + radius)];
	if ((int)(pos.x - radius) >= 0)
		cell[3] = &m->cells[(int)pos.y][(int)(pos.x - radius)];
	if (((cell[0] && cell[0]->is_goal) || (cell[1] && cell[1]->is_goal)
			|| (cell[2] && cell[2]->is_goal) || (cell[3] && cell[3]->is_goal))
		&& man->player.collected >= m->to_collect)
		change_game_state(man, GAME_STATE_SUCCESS);
	return ;
}

void	disable_collision_with_dst_portal_if_within(t_man *man, t_map *m,
	t_vec2 pos)
{
	static t_cell	*dst_portal_cell;
	static int		dst_portal_cell_is_obstacle;

	if (man->player.is_in_portal && m->cells)
	{
		if (!dst_portal_cell
			&& m->cells[(int)pos.y][(int)pos.x].portal)
		{
			dst_portal_cell = &m->cells[(int)pos.y][(int)pos.x];
			dst_portal_cell_is_obstacle = dst_portal_cell->is_obstacle;
			if (dst_portal_cell_is_obstacle)
				dst_portal_cell->is_obstacle = 0;
		}
		else if (!dst_portal_cell)
			man->player.is_in_portal = 0;
		else if (!m->cells[(int)pos.y][(int)pos.x].portal)
		{
			if (dst_portal_cell_is_obstacle)
				dst_portal_cell->is_obstacle = 1;
			dst_portal_cell = 0;
		}
	}
	return ;
}
