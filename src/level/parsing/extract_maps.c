#include "olafur.h"

static int	get_start_index(t_map *map);
static int	is_map_line(const char *s, int check_spec, int *only_spaces);
static int	is_char_in_set(char c, const char *set);
static int	skip_newlines(char **lines, int i);

int	extract_maps(t_map *map)
{
	int		start;
	int		end;
	int		i_maps;
	char	***maps[3];

	start = get_start_index(map);
	if (start < 0)
		return (0);
	maps[0] = &map->pars.map_wall;
	maps[1] = &map->pars.map_floor;
	maps[2] = &map->pars.map_ceil;
	i_maps = -1;
	while (++i_maps < 3 && map->pars.lines[start])
	{
		start = skip_newlines(map->pars.lines, start);
		end = start;
		while (map->pars.lines[end] && strcmp(map->pars.lines[end], "\n"))
			++end;
		*maps[i_maps] = (char **)extract_arr_elems((void **)map->pars.lines,
				start, end - 1);
	}
	start = skip_newlines(map->pars.lines, start);
	if (map->pars.lines[start])
		return (put_error(0, E_TOO_MANY_MAPS, 0, 0));
	return (1);
}

static int	get_start_index(t_map *map)
{
	int	i;
	int	index_maps;
	int	only_spaces;
	int	check_spec;

	i = -1;
	index_maps = -1;
	check_spec = 1;
	while (map->pars.lines[++i])
	{
		if (is_map_line(map->pars.lines[i], check_spec, &only_spaces))
		{
			if (index_maps < 0 && !only_spaces)
				index_maps = i;
		}
		else if (index_maps >= 0)
		{
			if (strcmp(map->pars.lines[i], "\n"))
				return (put_error(0, E_VARS_AFTER_MAP, 0, -1));
			check_spec = 0;
		}
	}
	if (index_maps < 0)
		return (put_error(0, E_NO_MAP, 0, -1));
	return (index_maps);
}

static int	is_map_line(const char *s, int check_spec, int *only_spaces)
{
	int	i;
	int	space;
	int	nbr_spaces;

	if (!s || s[0] == '\n')
		return (0);
	i = 0;
	nbr_spaces = 0;
	while (s[i])
	{
		space = !!isspace(s[i]);
		nbr_spaces += space;
		if (!space && !isdigit(s[i])
			&& (!check_spec || !is_char_in_set(s[i], MAP_CHARS)))
			return (0);
		++i;
	}
	if (only_spaces)
		*only_spaces = i == nbr_spaces;
	return (1);
}

static int	is_char_in_set(char c, const char *set)
{
	int	i;

	if (!set)
		return (0);
	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		++i;
	}
	return (0);
}

static int	skip_newlines(char **lines, int i)
{
	while (lines[i] && !strcmp(lines[i], "\n"))
		++i;
	return (i);
}
