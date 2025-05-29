#include "cimmerian.h"

void	free_maps(t_man *man)
{
	int	i;

	if (!man->maps)
		return ;
	i = 0;
	while (man->maps[i])
	{
		free_map(man->maps[i]);
		++i;
	}
	free(man->maps);
	man->maps = 0;
	return ;
}

void	free_map(t_map *map)
{
	int	i;

	if (!map)
		return ;
	free(map->filepath);
	free(map->map_walls);
	free(map->map_ceil_floor);
	free_cells(map);
	free_image(map->skybox, free);
	free_png(map->background);
	i = 0;
	while (i < map->portal_len)
	{
		free(map->portals[i].path_dst_map);
		++i;
	}
	free(map->portals);
	free_sprite_array(map);
	free(map);
	map = 0;
	return ;
}
