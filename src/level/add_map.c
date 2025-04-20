#include "cimmerian.h"

static t_map	*create_trap_map(const char *filepath);

/* Not a boolean. Returns the index of the map or -1 if failure. */
int	add_map(t_man *man, const char *filepath)
{
	int		index;
	t_map	**new_arr;
	size_t	count;

	if (!filepath)
		return (-1);
	index = find_map_index(man, filepath);
	if (index >= 0)
		return (index);
	count = count_arr_elems((void **)man->maps);
	new_arr = calloc(count + 1 + 1, sizeof(t_map *));
	if (!new_arr)
		return (-1);
	memcpy(new_arr, man->maps, count * sizeof(t_map *));
	free(man->maps);
	man->maps = new_arr;
	if (!strcmp(filepath, "null"))
		man->maps[count] = create_trap_map(filepath);
	else
		man->maps[count] = create_map(man, filepath);
	if (!man->maps[count])
		return (-1);
	return (count);
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

static t_map	*create_trap_map(const char *filepath)
{
	t_map	*map;

	map = calloc(1, sizeof(t_map));
	if (!map)
		return (0);
	map->filepath = strdup(filepath);
	if (!map->filepath)
	{
		free_map(map);
		return (0);
	}
	return (map);
}
