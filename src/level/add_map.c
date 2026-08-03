#include "olafur.h"

static int	find_map_index(t_man *man, const char *filepath);

/*
	Not a boolean. Returns the index of the map or -1 if failure.

	glfwPollEvents() is called so that, in case we add many maps in one go and 
	this takes some time, GLFW knows the program isn't freezing.
*/
int	add_map(t_man *man, const char *filepath)
{
	int		index;
	t_map	**new_arr;
	int		count;
	char	*abs_path;

	glfwPollEvents();
	abs_path = get_absolute_path(filepath);
	if (!abs_path)
		return (put_error(0, E_MAP_READ, filepath, -1));
	index = find_map_index(man, abs_path);
	if (index >= 0)
		return (free(abs_path), index);
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
	man->reset_dt = 1;
	return (count);
}

/* Add this map and all maps it leads to. */
int	add_map_and_submaps(t_man *man, const char *filepath)
{
	int		index;
	int		i;
	t_map	*map;

	index = add_map(man, filepath);
	if (index < 0)
		return (index);
	map = man->maps[index];
	if (map->submaps_added)
		return (index);
	map->submaps_added = 1;
	if (map->tp.map_path)
		add_map(man, map->tp.map_path);
	if (map->portals)
	{
		i = -1;
		while (map->portals[++i])
			add_map(man, map->portals[i]->path_dst_map);
	}
	return (index);
}

int	add_map_and_submaps_recursive(t_man *man, const char *filepath)
{
	int		index;
	int		i;
	t_map	*map;

	if (man->initial_map_loading_depth <= 0
		|| !filepath || !strcmp(filepath, "null"))
		return (-1);
	index = add_map(man, filepath);
	if (index < 0)
		return (index);
	map = man->maps[index];
	if (map->submaps_added)
		return (index);
	map->submaps_added = 1;
	--man->initial_map_loading_depth;
	if (map->tp.map_path)
		add_map_and_submaps_recursive(man, map->tp.map_path);
	if (map->portals)
	{
		i = -1;
		while (map->portals[++i])
			add_map_and_submaps_recursive(man, map->portals[i]->path_dst_map);
	}
	return (index);
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
