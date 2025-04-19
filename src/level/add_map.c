#include "cimmerian.h"

static t_map	*create_trap_map(t_man *man, const char *filepath);

int	add_map(t_man *man, const char *filepath)
{
	t_map	**new_arr;
	size_t	count;

	if (!filepath)
		put_error_and_exit(man, "", EXIT_FAILURE);
	if (find_map_index(man, filepath) >= 0)
		return (1);
	count = count_arr_elems((void **)man->maps);
	new_arr = calloc(count + 1 + 1, sizeof(t_map *));
	if (!new_arr)
		put_error_and_exit(man, "", EXIT_FAILURE);
	memcpy(new_arr, man->maps, count * sizeof(t_map *));
	free(man->maps);
	man->maps = new_arr;
	if (!strcmp(filepath, "null"))
		man->maps[count] = create_trap_map(man, filepath);
	else
		man->maps[count] = create_map(man, filepath);
	return (!!man->maps[count]);
}

int	find_map_index(t_man *man, const char *filepath)
{
	int	i;

	if (!man->maps || !filepath)
		return (-1);
	i = 0;
	while (man->maps[i])
	{
		if (!strcmp(man->maps[i]->filepath, filepath))
			return (i);
		++i;
	}
	return (-1);
}

static t_map	*create_trap_map(t_man *man, const char *filepath)
{
	t_map	*map;

	map = calloc(1, sizeof(t_map));
	if (!map)
		put_error_and_exit(man, "", EXIT_FAILURE);
	map->filepath = strdup(filepath);
	if (!map->filepath)
	{
		free_map(map);
		put_error_and_exit(man, "", EXIT_FAILURE);
	}
	return (map);
}
