#include "olafur.h"

static void	determine_map_size(t_ivec2 *size, char **map);
static int	give_same_height(t_ivec2 map_size, char ***map);
static int	give_same_width(t_ivec2 map_size, char **map);
static int	add_spaces(t_ivec2 map_size, char **map, int i);

int	make_maps_same_size(t_map *map)
{
	determine_map_size(&map->size, map->pars.map_wall);
	determine_map_size(&map->size, map->pars.map_floor);
	determine_map_size(&map->size, map->pars.map_ceil);
	return (give_same_height(map->size, &map->pars.map_wall)
		&& give_same_height(map->size, &map->pars.map_floor)
		&& give_same_height(map->size, &map->pars.map_ceil)
		&& give_same_width(map->size, map->pars.map_wall)
		&& give_same_width(map->size, map->pars.map_floor)
		&& give_same_width(map->size, map->pars.map_ceil));
}

static void	determine_map_size(t_ivec2 *size, char **map)
{
	t_ivec2	p;

	if (!map)
		return ;
	p.y = -1;
	while (map[++p.y])
	{
		p.x = -1;
		while (map[p.y][++p.x])
		{
			if (map[p.y][p.x] == '\n')
			{
				map[p.y][p.x] = 0;
				break ;
			}
		}
		size->x = imax(size->x, p.x);
	}
	size->y = imax(size->y, p.y);
	return ;
}

static int	give_same_height(t_ivec2 map_size, char ***map)
{
	int		height;
	char	**new_map;

	if (!*map)
		return (1);
	height = (int)count_arr_elems((void **)*map);
	if (height < map_size.y)
	{
		new_map = calloc(map_size.y + 1, sizeof(char *));
		if (!new_map)
			return (put_error(0, E_FAIL_MEM, 0, 0));
		memcpy(new_map, *map, height * sizeof(char *));
		free(*map);
		*map = new_map;
	}
	return (1);
}

static int	give_same_width(t_ivec2 map_size, char **map)
{
	int	i;

	if (!map)
		return (1);
	i = -1;
	while (++i < map_size.y)
	{
		if (!map[i])
		{
			map[i] = calloc(map_size.x + 1, sizeof(char));
			if (!map[i])
				return (put_error(0, E_FAIL_MEM, 0, 0));
			memset(map[i], ' ', map_size.x * sizeof(char));
		}
		else if (!add_spaces(map_size, map, i))
			return (0);
	}
	return (1);
}

static int	add_spaces(t_ivec2 map_size, char **map, int i)
{
	int		len;
	char	*new_str;

	len = (int)strlen(map[i]);
	if (len < map_size.x)
	{
		new_str = calloc(map_size.x + 1, sizeof(char));
		if (!new_str)
			return (put_error(0, E_FAIL_MEM, 0, 0));
		memcpy(new_str, map[i], len * sizeof(char));
		free(map[i]);
		map[i] = new_str;
		memset(map[i] + len, ' ', (map_size.x - len) * sizeof(char));
	}
	return (1);
}
