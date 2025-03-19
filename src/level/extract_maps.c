#include "cimmerian.h"

int	extract_maps(t_map *m, const char *filepath)
{
	char	*data;

	(void)m;
	data = read_file(filepath);
	if (!data)
		return (0);
	/**/printf("%s\n", data);
	/*
		- Delete `split_string` if you don't need it.
		- Count the size of `map_walls` and set the size in `t_map`.
		- Separate `map_walls` data to store it in `t_map->map_walls` without 
		the newlines.
		- Same thing for `map_buildings`.
	*/
	return (1);
}
