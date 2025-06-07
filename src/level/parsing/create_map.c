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
	if (!process_img_assets(man, map) || !process_sound_assets(man, map)
	   || !process_wall_types(man, map) || !process_spec_wall_types(man, map)
	   || !process_floor_types(man, map) || !process_ceil_types(man, map)
	   || !process_background_image(man, map) || !process_portals(man, map))
	{
		free_map(map);
		return (0);
	}
	/*
		TODO:
		- If an image is indicated as "null", make it actually null, this is 
		not an ID.
		- Remove a line once processed.
		- If a line remains at the end of the parsing, throw an error for an 
		unknown var and show its name.

		------------------------------------------------------------------------

		TODO: Test the portal parsing!! Otherwise the feature is done.

		PT
		-> Optional. Portal line.
		-> SRC_CARDINAL.
		-> SRC_POS_X.
		-> SRC_POS_Y.
		-> CLOSED_PORTAL_TEXTURE.
		-> OPEN_PORTAL_TEXTURE.
		-> DST_MAP. Don't check whether the dst map is legit. Trust.
		-> The dst transform is optional, but if info is given then we need all 
		three:
			-> DST_CARDINAL.
			-> DST_POS_X.
			-> DST_POS_Y.

		------------------------------------------------------------------------

		SP
		-> Optional. Sprite.
		-> Second token is the image, and it has to be there.
		-> After this token, and in any order, you can have "collectible" 
		and/or "collision". They are optional.
		-> After this line, each line of two tokens which are numbers are part 
		of this sprite. It's the XY position in double.
		-> If you find any other line, the processing of this sprite is done.

		------------------------------------------------------------------------

		-> Check the maps. Are they valid?
		-> If there's a [1-9]F but no floor map, give the texture to all 
		cells. If you have more than one type, pick the lowest.
		-> If there's a [1-9]C but no ceiling map, give the texture to all 
		cells. If you have more than one type, pick the lowest.
		-> In a floor or ceiling map, '0' means "show the background".

		------------------------------------------------------------------------

		- Allocate and set the cells.
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
