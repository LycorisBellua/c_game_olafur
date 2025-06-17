#include "olafur.h"

static int	check_top_and_bottom_edges(t_man *man, t_ivec2 *coord, t_ivec2 min,
				t_ivec2 max);
static int	check_left_and_right_edges(t_man *man, t_ivec2 *coord, t_ivec2 min,
				t_ivec2 max);

/*
   Use this function when the player is completely within a wall. It finds the 
   nearest available cell (within a limit of 10 circles for performance 
   reasons), and places the player right in the center of it. If no cell is 
   found, it's game over.
*/
void	unstuck_from_wall(t_man *man, t_map *m)
{
	t_ivec2	pos;
	t_ivec2	min;
	t_ivec2	max;
	t_ivec2	coord;
	int		radius;

	set_ivec2(&pos, (int)man->player.pos.x, (int)man->player.pos.y);
	if (pos.x < 0 || pos.y < 0 || pos.x >= m->size.x || pos.y >= m->size.y)
	{
		change_game_state(man, GAME_STATE_FAILURE);
		return ;
	}
	else if (!m->cells[pos.y][pos.x].is_obstacle)
		return ;
	radius = 0;
	while (++radius <= 10)
	{
		set_ivec2(&min, pos.x - radius, pos.y - radius);
		set_ivec2(&max, pos.x + radius, pos.y + radius);
		if (check_top_and_bottom_edges(man, &coord, min, max)
			|| check_left_and_right_edges(man, &coord, min, max))
			return ;
	}
	change_game_state(man, GAME_STATE_FAILURE);
	return ;
}

static int	check_top_and_bottom_edges(t_man *man, t_ivec2 *coord, t_ivec2 min,
	t_ivec2 max)
{
	t_map	*m;

	m = man->maps[man->curr_map];
	coord->x = min.x;
	while (coord->x <= max.x)
	{
		if (coord->x >= 0 && coord->x < m->size.x)
		{
			if (min.y >= 0 && min.y < m->size.y
				&& !m->cells[min.y][coord->x].is_obstacle)
			{
				set_vec2(&man->player.pos, coord->x + 0.5, min.y + 0.5);
				return (1);
			}
			if (max.y >= 0 && max.y < m->size.y
				&& !m->cells[max.y][coord->x].is_obstacle)
			{
				set_vec2(&man->player.pos, coord->x + 0.5, max.y + 0.5);
				return (1);
			}
		}
		++coord->x;
	}
	return (0);
}

static int	check_left_and_right_edges(t_man *man, t_ivec2 *coord, t_ivec2 min,
	t_ivec2 max)
{
	t_map	*m;

	m = man->maps[man->curr_map];
	coord->y = min.y + 1;
	while (coord->y < max.y)
	{
		if (coord->y >= 0 && coord->y < m->size.y)
		{
			if (min.x >= 0 && min.x < m->size.x
				&& !m->cells[coord->y][min.x].is_obstacle)
			{
				set_vec2(&man->player.pos, min.x + 0.5, coord->y + 0.5);
				return (1);
			}
			if (max.x >= 0 && max.x < m->size.x
				&& !m->cells[coord->y][max.x].is_obstacle)
			{
				set_vec2(&man->player.pos, max.x + 0.5, coord->y + 0.5);
				return (1);
			}
		}
		++coord->y;
	}
	return (0);
}
