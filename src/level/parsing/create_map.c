#include "cimmerian.h"

t_map	*create_map(t_man *man, const char *filepath)
{
	t_map	*map;

	map = fetch_map_data(filepath);
	if (!map)
		return (0);

	(void)man;
	free(map);
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

int	is_portal_visible(t_portal *portal)
{
	t_img	*tex;
	char	*tex_name;

	if (!portal || portal->is_corridor)
		return (0);
	if (portal->is_open && portal->tex_open)
		tex = portal->tex_open;
	else
		tex = portal->tex_closed;
	if (!tex)
		return (0);
	tex_name = strrchr(tex->id, '/');
	if (!tex_name)
		tex_name = tex->id;
	else
		++tex_name;
	return (tex_name && strncmp(tex_name, "null.png", 8));
}
