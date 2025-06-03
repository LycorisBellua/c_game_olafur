#include "cimmerian.h"

static char	*get_map(t_man *man, t_map *map, char **line);
static char	**fill_map(t_man *man, t_map *m, t_ivec2 size, char *lines);

char	**get_map_from_fd(t_man *man, t_map *map, char **line, int *n_texture)
{
	char	*lines;
	char	**new_map;

	lines = get_map(man, map, line);
	find_longest_line(lines, &(map->size), n_texture);
	new_map = fill_map(man, map, map->size, lines);
	free(lines);
	return (new_map);
}

void	get_maps(t_man *man, t_map *map, char **line, int *n_texture)
{
	int	n_maps;

	n_maps = -1;
	while (++n_maps != 2 || !(*line))
	{
		while (strncmp((*line), "map_", 4) != 0)
		{
			if (!onlyvalids((*line), WHITES))
				exit_in_parsing(man, map, E_WRONGCHAR, *line);
			free((*line));
			(*line) = gnl(map->fd);
		}
		if (!strncmp((*line), "map_walls", 9))
		{
			check_unfill_map(man, map, (*line), map->map_walls);
			map->map_walls = get_map_from_fd(man, map, line, n_texture);
		}
		else if (!strncmp((*line), "map_ceil_floor", 14))
		{
			check_unfill_map(man, map, (*line), map->map_ceil_floor);
			map->map_ceil_floor = get_map_from_fd(man, map, line, n_texture);
		}
	}
	if (map->start_pos.x < 0)
		exit_in_parsing(man, map, E_MISSPLAYER, NULL);
}

static char	*get_map(t_man *man, t_map *map, char **line)
{
	char	*tmp1;
	char	*tmp2;

	if (!man->bonus && !onlyvalids(*line, VALID_MANDA_MAP))
		exit_in_parsing(man, map, E_WRONGCHAR, *line);
	tmp1 = gnl(map->fd);
	skip_whiteline(map, &tmp1);
	if (!man->bonus && !onlyvalids(tmp1, VALID_MANDA_MAP))
		exit_in_parsing(man, map, E_WRONGCHAR, tmp1);
	tmp2 = gnl(map->fd);
	if (!tmp1 || !tmp2)
		exit_in_parsing(man, map, E_EMPTYLINE, *line);
	while (tmp2)
	{
		if (onlyvalids(tmp2, WHITES))
			break ;
		if (!man->bonus && !onlyvalids(tmp2, VALID_MANDA_MAP))
			exit_in_parsing(man, map, E_WRONGCHAR, tmp2);
		append(&tmp1, tmp2);
		tmp2 = gnl(map->fd);
	}
	free(*line);
	*line = tmp2;
	return (tmp1);
}

static char	**fill_map(t_man *man, t_map *m, t_ivec2 size, char *lines)
{
	char	**map;
	int		y;
	int		x;
	int		tmp;

	y = -1;
	x = 0;
	map = calloc(size.y + 1, sizeof(char *));
	if (!map)
		exit_in_parsing(man, m, E_FAIL_MEM, NULL);
	while (lines && ++y < size.y)
	{
		map[y] = calloc(size.x + 1, sizeof(char));
		if (!map[y])
		{
			free_arr((void **)map, free);
			exit_in_parsing(man, m, E_FAIL_MEM, NULL);
		}
		tmp = find_endline(lines + x);
		map[y] = ft_strncpy(map[y], lines + x, tmp);
		x += tmp;
		find_player(man, m, map[y], y);
		x++;
	}
	return (map);
}
