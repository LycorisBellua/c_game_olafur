#include "cimmerian.h"

void	audio_source_delete_map(t_map *map)
{
	if (!map || !map->sources)
		return ;
	while (map->sources[0])
	{
		audio_source_delete(&map->sources[0]);
		remove_arr_elems((void **)map->sources, 0, 0, free);
	}
	free(map->sources);
	map->sources = 0;
	return ;
}
