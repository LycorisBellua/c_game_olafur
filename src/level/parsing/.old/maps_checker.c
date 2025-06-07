#include "cimmerian.h"

/*
static void	check_adjacent_floor_tiles(t_man *man, t_map *m, char **map,
				t_vec2 pos);

void	valid_map(t_man *man, t_map *map)
{
	t_vec2	p;

	check_adjacent_floor_tiles(man, map, map->map_walls, map->start_pos);
	if (!man->bonus && (!map->types[1].tex_north || !map->types[1].tex_south
			|| !map->types[1].tex_east || !map->types[1].tex_west
			|| map->floor_color.a != 255 || map->ceiling_color.a != 255))
		exit_in_parsing(man, map, E_MISSINFO, NULL);
	p.y = -1;
	while (++p.y < map->size.y)
	{
		if (map->map_walls[(int)p.y][0] == '\n'
			|| !onlyvalids(map->map_walls[(int)p.y], VALID_MANDATORY))
			exit_in_parsing(man, map, E_INVALIDCHAR, NULL);
		p.x = -1;
		while (++p.x < map->size.x)
		{
			if (map->map_walls[(int)p.y][(int)p.x] == '0')
				check_adjacent_floor_tiles(man, map, map->map_walls, p);
		}
	}
}

void	check_unfill_map(t_man *man, t_map *map, char *ln, char **map_to_check)
{
	if (map_to_check)
		exit_in_parsing(man, map, E_DOUBLEMAP, ln);
}

void	maps_symetry(t_man *man, t_map *map)
{
	int	y;
	int	x;

	y = 0;
	if (!map->map_walls || !map->map_ceil_floor)
		exit_in_parsing(man, map, E_EMPTYMAP, NULL);
	while (map->map_walls[y])
	{
		x = 0;
		while (map->map_walls[y][x])
		{
			if (isvalid(map->map_walls[y][x], VALID_BONUS))
			{
				if (!isvalid(map->map_ceil_floor[y][x], VALID_BONUS))
					exit_in_parsing(man, map, E_INVALIDCHAR, NULL);
			}
			else
			{
				if (isvalid(map->map_ceil_floor[y][x], VALID_BONUS))
					exit_in_parsing(man, map, E_INVALIDCHAR, NULL);
			}
			x++;
		}
		y++;
	}
}

static void	check_adjacent_floor_tiles(t_man *man, t_map *m, char **map,
t_vec2 pos)
{
	if ((int)pos.y > 0 && (int)pos.y < m->size.y - 1 && (int)pos.x > 0
		&& (int)pos.x < m->size.x - 1)
	{
		if (!isvalid(map[(int)pos.y - 1][(int)pos.x], &VALID_MANDATORY[1]))
			exit_in_parsing(man, m, E_UPTILE, NULL);
		if (!isvalid(map[(int)pos.y + 1][(int)pos.x], &VALID_MANDATORY[1]))
			exit_in_parsing(man, m, E_UNDTILE, NULL);
		if (!isvalid(map[(int)pos.y][(int)pos.x - 1], &VALID_MANDATORY[1]))
			exit_in_parsing(man, m, E_LFTTILE, NULL);
		if (!isvalid(map[(int)pos.y][(int)pos.x + 1], &VALID_MANDATORY[1]))
			exit_in_parsing(man, m, E_RGTTILE, NULL);
	}
	else
	{
		exit_in_parsing(man, m, E_OUTTILE, NULL);
	}
}
*/
