#include "cimmerian.h"

static void	free_cell_arrays(t_map *map);
static void	free_portal_array(t_map *map);

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
	if (!map)
		return ;
	release_parsing_data(map);
	free(map->filepath);
	free_image(map->skybox, free);
	free_png(map->background);
	free_cell_arrays(map);
	free_portal_array(map);
	free_sprite_array(map);
	free(map);
	map = 0;
	return ;
}

void	free_sprite_array(t_map *map)
{
	int	i;

	if (!map)
		return ;
	if (map->sprites)
	{
		i = 0;
		while (map->sprites[i])
		{
			free(map->sprites[i]);
			++i;
		}
		free(map->sprites);
	}
	map->sprites = 0;
	map->sprite_len = 0;
	return ;
}

static void	free_cell_arrays(t_map *map)
{
	t_ivec2	coord;

	if (!map)
		return ;
	if (map->cells)
	{
		coord.y = 0;
		while (coord.y < map->size.y)
		{
			coord.x = 0;
			while (coord.x < map->size.x)
			{
				free(map->cells[coord.y][coord.x].door);
				++coord.x;
			}
			free(map->cells[coord.y]);
			++coord.y;
		}
		free(map->cells);
	}
	map->cells = 0;
	return ;
}

static void	free_portal_array(t_map *map)
{
	int	i;

	if (!map || !map->portals)
		return ;
	i = 0;
	while (map->portals[i])
	{
		free(map->portals[i]->path_dst_map);
		free(map->portals[i]);
		++i;
	}
	free(map->portals);
	map->portals = 0;
	return ;
}
