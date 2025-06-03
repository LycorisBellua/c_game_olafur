#include "cimmerian.h"

void	go_to_mandatory(char *line, int *bonus)
{
	if (!strncmp(line, "NO", 2) || !strncmp(line, "SO", 2)
		|| !strncmp(line, "WE", 2) || !strncmp(line, "EA", 2)
		|| !strncmp(line, "F", 1) || !strncmp(line, "C", 1))
	{
		*bonus = 0;
	}
}

t_map	*create_map(t_man *man, const char *file)
{
	t_map	*map;
	int		fd;
	char	*line;

	line = NULL;
	fd = check_file(man, file);
	line = gnl(fd);

	while (line && !strncmp(line, "JSON ", 5))
	{
		free(line);
		line = gnl(fd);
	}
	free(line);
	line = gnl(fd);
	/*
	while (line && !strncmp(line, "JSON ", 5))
	{
		line[strlen(line) - 1] = '\0';
		if (!update_image_array(man, line + 5))
		{
			// abort map creation
		}
		free(line);
		line = gnl(fd);
	}
	*/

	if (man->bonus)
		go_to_mandatory(line, &man->bonus);
	malloc_struct(man, &map, fd, file);
	parse_file_map(man, map, &line);
	return (map);
}

void	parse_bonus_lines(t_man *man, t_map *map, char **line)
{
	if (!man->bonus)
		return ;
	find_door_goal(man, map, line);
	find_skybox(man, map, line);
	find_portals(man, map, line);
	find_sprites(man, map, line);
	find_music(man, map, line);
}
