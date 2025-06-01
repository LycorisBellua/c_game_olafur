#include "cimmerian.h"

void	malloc_struct(t_man *man, t_map **map, int fd, const char *file)
{
	int	nbr_types;

	*map = calloc(1, sizeof(t_map));
	if (!*map)
	{
		close(fd);
		exit_in_parsing(man, NULL, E_ALLOCFAILMAP, NULL);
	}
	(*map)->fd = fd;
	(*map)->filepath = get_absolute_path(file);
	if (!(*map)->filepath)
		exit_in_parsing(man, *map, E_ALLOCFAILMAP, NULL);
	nbr_types = 1;
	if (man->bonus)
		nbr_types = 9;
	(*map)->types = calloc(nbr_types + 1, sizeof(t_cell));
	if (!(*map)->types)
		exit_in_parsing(man, *map, E_ALLOCFAILTYPES, NULL);
	set_vec2(&(*map)->start_pos, -1, -1);
	(*map)->fog_color = get_color_rgba(0, 0, 0, 255);
}

void	exit_in_parsing(t_man *man, t_map *map, char *msg, void *data)
{
	free(data);
	if (map->fd != -1)
		close(map->fd);
	if (map)
		free_map(map);
	put_error_and_exit(man, msg, 1, EXIT_FAILURE);
}

int	check_file(t_man *man, const char *filename)
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
	if (fd == -1)
		put_error_and_exit(man, E_NOFILEMAP, 1, EXIT_FAILURE);
	return (fd);
}

void	reach_endfile(t_man *man, t_map *map, char **line)
{
	char	*tmp;

	if (!*line)
	{
		close(map->fd);
		return ;
	}
	tmp = strdup(*line);
	free(*line);
	while (tmp)
	{
		if (onlyvalids(tmp, WHITES) == 0)
		{
			free(tmp);
			exit_in_parsing(man, map, E_WRONGCHAR, NULL);
		}
		free(tmp);
		tmp = gnl(map->fd);
	}
	close(map->fd);
}
