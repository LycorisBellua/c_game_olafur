#include "cimmerian.h"

static int		is_valid_file(const char *filepath);
static t_map	*alloc_map_and_read_lines(const char *filepath);
static int		is_map_line(const char *s);
static int		has_only_whitespaces(const char *s);

t_map	*fetch_map_data(const char *filepath)
{
	t_map	*map;

	if (!is_valid_file(filepath))
		return (0);
	map = alloc_map_and_read_lines(filepath);
	if (!map)
		return (0);
	
	int i = 0;
	int	is_map_found = 0;
	while (map->pars.info[i])
	{
		if (is_map_line(map->pars.info[i]))
		{
			if (!is_map_found && !has_only_whitespaces(map->pars.info[i]))
				is_map_found = 1;
		}
		else if (is_map_found && strcmp(map->pars.info[i], "\n"))
		{
			printf("INFO FOUND AFTER MAP CONTENT\n");
			break ;
		}
		++i;
	}
	/*
		- Cut each line into tokens at whitespaces. If a line is only 
		whitespaces, remove it. Except if it's map data. The map needs to be 
		at the end. Basically, before tokenizing, if a line only has characters 
		allowed in a map, it's a map line. After the first map line, if you 
		find newlines it's okay, ignore it for the time being, but if you find 
		other characters which are not allowed in a map, return an error which 
		explains that the maps need to come last (wall map first, then floor, 
		then ceiling). Only the wall map is necessary, the other two are 
		optional. Also throw an error at this step if there are more than 3 
		maps or no map at all. At this point, we know that all the info lines 
		come first, and then come the maps, and that we have the precise amount 
		of maps we want (between 1 and 3).

		- Browse the entire file. For each line which starts with "JSON" as the 
		first token, call `update_image_array(man, second_token)`. Abort the 
		map creation if it returns false.
	*/

	/*
		- At which index does the map content start and end? We need three set 
		of indexes: wall map, floor map, ceiling map. Initialize them as -1. 
		The floor and ceiling maps are optional. And they are always in this 
		specific order, their nature is not determined by their content.

		//map->pars.map_wall
		typedef struct s_map_pars
		{
			char		**info;
			char		**map_wall;
			char		**map_floor;
			char		**map_ceil;
			t_tex_type	*types;
		}	t_map_pars;

		Extract the map lines into the map arrays, and remove them from the 
		original lines.

		Then, only info lines remain. Remove empty lines as they are not 
		relevant. The only relevant empty lines are between map1 and map2, and 
		map2 and map3. Indeed, it's okay not to have an empty line between the 
		info and map1.
	*/
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
			map->pars.info = read_file_lines(filepath);
	}
	if (!map || !map->filepath || !map->pars.info)
	{
		free_map(map);
		put_error(0, E_FAIL_MEM, "alloc_map_and_read_lines", 0);
		map = 0;
	}
	return (map);
}

static int	is_map_line(const char *s)
{
	int	i;
	int	j;

	if (!s)
		return (1);
	i = 0;
	while (s[i])
	{
		if (!i && s[i] == '\n')
			return (0);
		else if (!isspace(s[i]) && !isdigit(s[i]))
		{
			j = 0;
			while (MAP_CHARS[j])
			{
				if (s[i] == MAP_CHARS[j])
					break ;
				++j;
			}
			if (!MAP_CHARS[j])
				return (0);
		}
		++i;
	}
	return (1);
}

static int	has_only_whitespaces(const char *s)
{
	int	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i] && isspace(s[i]))
		++i;
	return (!s[i]);
}
