#include "cimmerian.h"

t_map	*create_map(t_man *man, const char *filepath)
{
	t_map	*map;

	/*
		TODO
		- If a line ends in "\" (with a newline of course), do we concatenate?
		- And if so, do we do this in the parsing or in the original file 
		reading function?

		- Add a README to the maps folder to explain how to format a map file.
	*/
	map = fetch_map_data(filepath);
	if (!map)
		return (0);
	else if (!process_img_assets(man, map) || !process_sound_assets(man, map)
	   || !process_wall_types(man, map) || !process_spec_wall_types(man, map)
	   || !process_floor_types(man, map) || !process_ceil_types(man, map)
	   || !process_background_image(man, map) || !process_portals(man, map)
	   || !process_sprites(man, map) || !is_var_list_empty(map))
	{
		free_map(map);
		return (0);
	}

	/*
		TODO:

		- Check all three maps.
			-> If there's an unknown character, it's bad.
			-> If the character is a digit greater than '0', and there's no 
			wall/floor/ceil type for this symbol, it's bad. Same thing for D 
			and G if found in the map.
			-> Find longest line amongst all three maps. This will be the map's 
			`size.x`.
			-> Find the map with the most lines, this will be `size.y`.

		- In map walls:
			-> Check if a '0' cell is surrounded by a digit or "NSWEDG", 
			otherwise it's bad. It's also bad if it's on a map edge (= there's 
			no character).
			-> Only check top, bottom, left and right.

		------------------------------------------------------------------------

		- Allocate and set the cells.
		- In a floor or ceiling map, '0' means "show the background".

		------------------------------------------------------------------------

		- Free `map->pars` and nullify everything to avoid a double free in 
		`free_map` later on. We free `pars` in advance to free some space.
	*/
	free_map(map);
	return (0);
}

/*
void	parse_file_map(t_man *man, t_map *map, char **line)
{
	valid_map(man, map);
	//
	get_maps(man, map, line, &n_texture);
	maps_symetry(man, map);
}
*/
