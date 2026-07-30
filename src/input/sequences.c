#include "olafur.h"

static void	teleport(t_man *man);

void	konami_code(t_man *man, int pressed_key)
{
	static int	sequence[10];
	static int	index;

	if (!sequence[0])
	{
		sequence[0] = KEY_UP;
		sequence[1] = KEY_UP;
		sequence[2] = KEY_DOWN;
		sequence[3] = KEY_DOWN;
		sequence[4] = KEY_LEFT;
		sequence[5] = KEY_RIGHT;
		sequence[6] = KEY_LEFT;
		sequence[7] = KEY_RIGHT;
		sequence[8] = KEY_B;
		sequence[9] = KEY_A;
	}
	if (pressed_key != sequence[index++])
		index = 0;
	if (index == sizeof(sequence) / sizeof(int))
	{
		index = 0;
		teleport(man);
	}
	return ;
}

static void	teleport(t_man *man)
{
	t_map	*map;
	int		index_tp;
	t_ivec2	pos;

	map = man->maps[man->curr_map];
	if (!map->tp_path_map)
		return ;
	index_tp = add_map(man, map->tp_path_map);
	if (index_tp < 0 || index_tp == man->curr_map)
		return ;
	man->curr_map = index_tp;
	map = man->maps[man->curr_map];
	set_ivec2(&pos, man->player.pos.x, man->player.pos.y);
	if (pos.x >= map->size.x || pos.y >= map->size.y
		|| map->cells[pos.y][pos.x].is_obstacle)
		set_player_transform(man, map->start_pos, man->player.dir);
	return ;
}
