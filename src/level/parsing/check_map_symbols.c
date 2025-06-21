#include "olafur.h"

static int	gather_types(char **map, int types[11]);
static int	check_types_w(t_map *map, t_wall_type *tex, int types[11]);
static int	check_types_fc(t_row_type *tex, int types[11]);

int	check_map_symbols(t_map *map)
{
	int	w_types[11];
	int	f_types[11];
	int	c_types[11];

	bzero(w_types, 11 * sizeof(int));
	bzero(f_types, 11 * sizeof(int));
	bzero(c_types, 11 * sizeof(int));
	if (map->pars.map_wall
		&& gather_types(map->pars.map_wall, w_types)
		&& !check_types_w(map, map->pars.tex_types_wall, w_types))
		return (put_error(0, E_BAD_SYM_W, 0, 0));
	else if (map->pars.map_floor
		&& gather_types(map->pars.map_floor, f_types)
		&& !check_types_fc(map->pars.tex_types_floor, f_types))
		return (put_error(0, E_BAD_SYM_F, 0, 0));
	else if (map->pars.map_ceil
		&& gather_types(map->pars.map_ceil, c_types)
		&& !check_types_fc(map->pars.tex_types_ceil, c_types))
		return (put_error(0, E_BAD_SYM_C, 0, 0));
	return (1);
}

static int	gather_types(char **map, int types[11])
{
	t_ivec2	p;

	p.y = -1;
	while (map[++p.y])
	{
		p.x = -1;
		while (map[p.y][++p.x])
		{
			if (isdigit(map[p.y][p.x]))
				types[map[p.y][p.x] - '0'] = 1;
			else if (map[p.y][p.x] == 'G')
				types[10] = 1;
		}
	}
	return (1);
}

static int	check_types_w(t_map *map, t_wall_type *tex, int types[11])
{
	int	i;
	int	j;

	if (types[10] && !map->pars.goal_line_found)
		return (0);
	i = 1;
	while (i < 9)
	{
		if (types[i])
		{
			j = 0;
			while (tex && tex[j].symbol)
			{
				if (tex[j].symbol == ('0' + i))
					break ;
				++j;
			}
			if (!tex || !tex[j].symbol)
				return (0);
		}
		++i;
	}
	return (1);
}

static int	check_types_fc(t_row_type *tex, int types[11])
{
	int	i;
	int	j;

	i = 1;
	while (i < 9)
	{
		if (types[i])
		{
			j = 0;
			while (tex && tex[j].symbol)
			{
				if (tex[j].symbol == ('0' + i))
					break ;
				++j;
			}
			if (!tex || !tex[j].symbol)
				return (0);
		}
		++i;
	}
	return (1);
}
