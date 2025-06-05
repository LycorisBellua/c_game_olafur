#include "cimmerian.h"

static int		is_valid_file(const char *filepath);
static t_map	*alloc_map_and_read_lines(const char *filepath);
static int		tokenize_var_lines(t_map *map);
static int		remove_whitespace_only_lines(t_map *map);

t_map	*fetch_map_data(const char *filepath)
{
	t_map	*map;

	if (!is_valid_file(filepath))
		return (0);
	map = alloc_map_and_read_lines(filepath);
	if (!map)
		return (0);
	if (!extract_maps(map) || !tokenize_var_lines(map))
	{
		free_map(map);
		return (0);
	}
	return (map);
}

static int	is_valid_file(const char *filepath)
{
	struct stat	st;
	const char	*name;
	size_t		len;

	if (!filepath || access(filepath, R_OK) < 0 || stat(filepath, &st) < 0
		|| !S_ISREG(st.st_mode))
		return (put_error(0, E_MAP_READ, filepath, 0));
	name = get_filename(filepath);
	len = strlen(name);
	if (len < 4 || strcmp(name + len - 4, ".map"))
		return (put_error(0, E_MAP_TYPE, filepath, 0));
	if (len < 5)
		return (put_error(0, E_MAP_NAME, filepath, 0));
	return (1);
}

static t_map	*alloc_map_and_read_lines(const char *filepath)
{
	t_map	*map;

	map = calloc(1, sizeof(t_map));
	if (map)
	{
		map->filepath = strdup(filepath);
		if (map->filepath)
			map->pars.lines = read_file_lines(filepath);
	}
	if (!map || !map->filepath || !map->pars.lines)
	{
		free_map(map);
		put_error(0, E_FAIL_MEM, "alloc_map_and_read_lines", 0);
		map = 0;
	}
	return (map);
}

static int	tokenize_var_lines(t_map *map)
{
	int	i;
	int	len;

	len = remove_whitespace_only_lines(map);
	if (!map->pars.lines || !map->pars.lines[0])
		return (put_error(0, E_NO_VAR, 0, 0));
	map->pars.vars = calloc(len + 1, sizeof(char **));
	if (!map->pars.vars)
		return (put_error(0, E_FAIL_MEM, "tokenize_var_lines", 0));
	i = 0;
	while (map->pars.lines[i])
	{
		map->pars.vars[i] = split(map->pars.lines[i], ' ');
		if (!map->pars.vars[i])
			return (put_error(0, E_FAIL_MEM, "tokenize_var_lines", 0));
		++i;
	}
	free_arr((void **)map->pars.lines, free);
	map->pars.lines = 0;
	return (1);
}

static int	remove_whitespace_only_lines(t_map *map)
{
	int	i;
	int	j;
	int	nbr_spaces;

	if (!map->pars.lines)
		return (0);
	i = 0;
	while (map->pars.lines[i])
	{
		j = -1;
		nbr_spaces = 0;
		while (map->pars.lines[i][++j])
		{
			if (isspace(map->pars.lines[i][j]) || map->pars.lines[i][j] == ',')
			{
				++nbr_spaces;
				map->pars.lines[i][j] = ' ';
			}
		}
		if (j == nbr_spaces)
			remove_arr_elems((void **)map->pars.lines, i, i, free);
		else
			++i;
	}
	return (i);
}
