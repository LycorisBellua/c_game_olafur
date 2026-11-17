#include "olafur.h"

static int	check_key_sequence(t_map *map, int key);
static int	is_start_matched(t_teleport *tp, int key, int len);

void	teleport_through_key_sequence(t_man *man, int key)
{
	t_map	*map;
	int		index_tp;
	t_ivec2	pos;

	map = man->maps[man->curr_map];
	if (!map->tp.map_path || !check_key_sequence(map, key))
		return ;
	index_tp = add_map_and_submaps(man, map->tp.map_path);
	if (index_tp < 0 || index_tp == man->curr_map)
		return ;
	man->curr_map = index_tp;
	map = man->maps[man->curr_map];
	set_ivec2(&pos, man->player.pos.x, man->player.pos.y);
	if (pos.x >= map->size.x || pos.y >= map->size.y
		|| map->cells[pos.y][pos.x].is_obstacle)
		set_player_transform(man, map->start_pos, man->player.dir);
	else
		audio_switch_map_sounds(man);
	return ;
}

static int	check_key_sequence(t_map *map, int key)
{
	int	len;

	len = map->tp.seq_index + 1;
	while (len > 0 && !is_start_matched(&map->tp, key, len))
		--len;
	map->tp.seq_index = len;
	if (map->tp.seq_index == map->tp.seq_len)
	{
		map->tp.seq_index = 0;
		return (1);
	}
	return (0);
}

static int	is_start_matched(t_teleport *tp, int key, int len)
{
	int	i;

	if (tp->sequence[len - 1] != key)
		return (0);
	i = 0;
	while (i < len - 1)
	{
		if (tp->sequence[i] != tp->sequence[tp->seq_index - len + 1 + i])
			return (0);
		++i;
	}
	return (1);
}
