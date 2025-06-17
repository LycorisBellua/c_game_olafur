#include "olafur.h"

static int	find_map_index(t_man *man, const char *filepath);

/* Not a boolean. Returns the index of the map or -1 if failure. */
int	add_map(t_man *man, const char *filepath)
{
	int		index;
	t_map	**new_arr;
	int		count;
	char	*abs_path;

	abs_path = get_absolute_path(filepath);
	if (!abs_path)
		return (put_error(0, E_MAP_READ, filepath, -1));
	index = find_map_index(man, abs_path);
	if (index >= 0)
	{
		free(abs_path);
		return (index);
	}
	count = count_arr_elems((void **)man->maps);
	new_arr = calloc(count + 1 + 1, sizeof(t_map *));
	if (!new_arr)
		return (-1);
	memcpy(new_arr, man->maps, count * sizeof(t_map *));
	free(man->maps);
	man->maps = new_arr;
	man->maps[count] = create_map(man, abs_path);
	free(abs_path);
	if (!man->maps[count])
		return (-1);
	return (count);
}

static int	find_map_index(t_man *man, const char *filepath)
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
