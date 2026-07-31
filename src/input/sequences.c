#include "olafur.h"

static int	check_key_sequence(t_map *map, int key);

void	teleport_through_key_sequence(t_man *man, int key)
{
	t_map	*map;
	int		index_tp;
	t_ivec2	pos;

	map = man->maps[man->curr_map];
	if (!map->tp.map_path || !check_key_sequence(map, key))
		return ;
	index_tp = add_map(man, map->tp.map_path);
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

static int	check_key_sequence(t_map *map, int key)
{
	if (key != map->tp.sequence[map->tp.seq_index++])
		map->tp.seq_index = 0;
	if (map->tp.seq_index == map->tp.seq_len)
	{
		map->tp.seq_index = 0;
		return (1);
	}
	return (0);
}
