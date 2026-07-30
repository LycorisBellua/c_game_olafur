#include "olafur.h"

static void	free_cell_arrays(t_map *map);
static void	free_portal_array(t_map *map);

void	free_maps(t_man *man)
{
	int	i;

	free(man->first_map_path);
	man->first_map_path = 0;
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

static void	free_bg_frames(t_map *map)
{
	int	i;

	if (map->bg_frames)
	{
		i = -1;
		while (++i < map->bg_frame_count)
			free_png(map->bg_frames[i]);
		free(map->bg_frames);
		map->bg_frames = 0;
	}
	map->background = 0;
	return ;
}

void	free_map(t_map *map)
{
	if (!map)
		return ;
	release_parsing_data(map);
	free(map->filepath);
	free(map->tp_path_map);
	free_image(map->skybox, free);
	free_bg_frames(map);
	free_cell_arrays(map);
	free_arr((void **)map->doors, free);
	free_portal_array(map);
	free_sprite_array(map);
	audio_source_delete_map(map);
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
	if (!map)
		return ;
	if (map->cells)
	{
		if (map->size.y > 0)
			free(map->cells[0]);
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
