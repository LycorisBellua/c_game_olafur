#include "cimmerian.h"

void	parse_file_map(t_man *man, t_map *map, char **line)
{
	int	n_texture;

	n_texture = 0;
	if (!(*line))
		exit_in_parsing(man, map, E_EMPTYMAP, NULL);
	if (!man->bonus)
	{
		find_cardinals_floor_ceiling(man, map, line, n_texture);
		map->map_walls = get_map_from_fd(man, map, line, &n_texture);
		reach_endfile(man, map, line);
		valid_map(man, map);
		compose_skybox(man, map, NULL);
		fill_cells(man, map);
	}
	else
	{
		get_maps(man, map, line, &n_texture);
		maps_symetry(man, map);
		find_cardinals_floor_ceiling(man, map, line, n_texture);
		parse_bonus_lines(man, map, line);
		reach_endfile(man, map, line);
		fill_cells(man, map);
	}
	compose_background(man, map);
	close(map->fd);
}
