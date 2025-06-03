#include "cimmerian.h"

static void	parse_bonus_lines(t_man *man, t_map *map, char **line);
static void	reach_endfile(t_man *man, t_map *map, char **line);

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

static void	parse_bonus_lines(t_man *man, t_map *map, char **line)
{
	if (!man->bonus)
		return ;
	find_door_goal(man, map, line);
	find_skybox(man, map, line);
	find_portals(man, map, line);
	find_sprites(man, map, line);
	find_music(man, map, line);
}

static void	reach_endfile(t_man *man, t_map *map, char **line)
{
	char	*tmp;

	if (!*line)
	{
		close(map->fd);
		return ;
	}
	tmp = strdup(*line);
	free(*line);
	while (tmp)
	{
		if (!onlyvalids(tmp, WHITES))
		{
			free(tmp);
			exit_in_parsing(man, map, E_WRONGCHAR, NULL);
		}
		free(tmp);
		tmp = gnl(map->fd);
	}
	close(map->fd);
}
