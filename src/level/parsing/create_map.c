#include "cimmerian.h"

t_map	*create_map(t_man *man, const char *filepath)
{
	char	**lines;

	/*
		# define E_MAP_READ "The map file cannot be read"
			-> string is null || file doesn't exist || is not a file (dir) || no read perm
		# define E_MAP_TYPE "The map file doesn't have the *.map extension"
			-> Needs to end in ".map"
		# define E_MAP_NAME "The map file doesn't have a name"
			-> no character before the last '.'
		
		return (put_error(0, E_MAP_TYPE, filepath, 0));
	*/
		

	lines = read_file_lines(filepath);
	if (!lines)
		return (0);
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

	return (0);
}

/*
t_map	*create_map(t_man *man, const char *filepath)
{
	t_map	*map;
	int		fd;
	char	*line;

	line = NULL;
	fd = check_file(man, filepath);
	line = gnl(fd);
	while (line && !strncmp(line, "JSON ", 5))
	{
		free(line);
		line = gnl(fd);
	}
	free(line);
	line = gnl(fd);
	if (man->bonus)
		go_to_mandatory(line, &man->bonus);
	malloc_struct(man, &map, fd, filepath);
	parse_file_map(man, map, &line);
	return (map);
}

static int	check_file(t_man *man, const char *filename)
{
	int	size;
	int	check;
	int	fd;

	size = strlen(filename);
	check = revncmp(filename, ".map", 4);
	if (size < 5 || check != 0)
		put_error_and_exit(man, E_WRONGTYPE, 1, EXIT_FAILURE);
	fd = open(filename, O_RDONLY | __O_DIRECTORY);
	if (fd > 0)
		put_error_and_exit(man, E_DIRREFUSED, 1, EXIT_FAILURE);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		put_error_and_exit(man, E_NOFILEMAP, 1, EXIT_FAILURE);
	return (fd);
}

static void	go_to_mandatory(char *line, int *bonus)
{
	if (!strncmp(line, "NO", 2) || !strncmp(line, "SO", 2)
		|| !strncmp(line, "WE", 2) || !strncmp(line, "EA", 2)
		|| !strncmp(line, "F", 1) || !strncmp(line, "C", 1))
	{
		*bonus = 0;
	}
}

static void	malloc_struct(t_man *man, t_map **map, int fd, const char *file)
{
	int	nbr_types;

	*map = calloc(1, sizeof(t_map));
	if (!*map)
	{
		close(fd);
		exit_in_parsing(man, NULL, E_FAIL_MEM, NULL);
	}
	(*map)->fd = fd;
	(*map)->filepath = get_absolute_path(file);
	if (!(*map)->filepath)
		exit_in_parsing(man, *map, E_FAIL_MEM, NULL);
	nbr_types = 1;
	if (man->bonus)
		nbr_types = 9;
	(*map)->types = calloc(nbr_types + 1, sizeof(t_cell));
	if (!(*map)->types)
		exit_in_parsing(man, *map, E_FAIL_MEM, NULL);
	set_vec2(&(*map)->start_pos, -1, -1);
	(*map)->fog_color = get_color_rgba(0, 0, 0, 255);
}
*/
