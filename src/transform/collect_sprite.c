#include "cimmerian.h"

int	collect_sprite(t_man *man, int sprite_index)
{
	int		i;
	t_map	*map;

	map = man->maps[man->curr_map];
	if (!map->sprites[sprite_index]->is_collectible
		|| man->player.collected >= man->player.to_collect)
		return (0);
	free(map->sprites[sprite_index]);
	i = sprite_index;
	while (i < map->sprite_len - 1)
	{
		map->sprites[i] = map->sprites[i + 1];
		++i;
	}
	--map->sprite_len;
	++man->player.collected;
	return (1);
}
