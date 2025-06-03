#include "cimmerian.h"

void	exit_in_parsing(t_man *man, t_map *map, char *msg, void *data)
{
	free(data);
	if (map->fd != -1)
		close(map->fd);
	if (map)
		free_map(map);
	put_error_and_exit(man, msg, 1, EXIT_FAILURE);
}
