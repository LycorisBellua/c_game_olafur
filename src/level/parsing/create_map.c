#include "cimmerian.h"

static int	process_json_lines(t_man *man, t_map *map);

t_map	*create_map(t_man *man, const char *filepath)
{
	t_map	*map;

	/*
		TODO
		- If a line ends in "\" (with a newline of course), do we concatenate?
		- And if so, do we do this in the parsing or in the original file 
		reading function?
	*/
	map = fetch_map_data(filepath);
	if (!map)
		return (0);
	if (!process_json_lines(man, map))
	{
		free(map);
		return (0);
	}
	/*
		TODO:
		- Process var lines. Remove a line once processed.
		- If a line remains at the end of the parsing, throw an error for an 
		unknown var and show its name.
		- If an image is indicated as "null", make it actually null, this is 
		not an ID.

		NO SO WE EA + version with digits
		-> Any order goes, and they don't have to follow each other.
		-> No duplicate.
		-> Need all four of a given type.
		-> If the type isn't indicated, it's type '1'.
		-> The type is one digit between 1 and 9.
		-> If no second token, it's bad.
		-> If third token, it's bad.
		-> If the second token's processing fails in `add_new_image`, it's bad.

		F C
		-> Initialize the floor, ceiling and fog colors to (0, 0, 0, 255).
		-> Can be an RGB color. Alpha is not indicated and is 255.
		-> Can be a hex color. Alpha is not indicated and is 255.
		-> Is optional, any order goes and they don't have to follow each other.
		-> No duplicate.

		F C but with digits (1 -> 9)
		-> Is optional, any order goes and they don't have to follow each other.
		-> No duplicate.
		-> If there's an F but no floor map, give the texture to all cells. If 
		you have more than one type, pick the lowest.
		-> If there's a C but no ceiling map, give the texture to all cells. If 
		you have more than one type, pick the lowest.
		-> In a floor or ceiling map, '0' means "show the background".

		D
		-> Optional. Texture for the door cell's walls.
		-> First comes the closed version.
		-> Then, an optional open version.
		
		G
		-> Optional. Texture for the goal cell's walls.

		S
		-> Optional. Skybox image.
		-> Once you have it, compose the background and stuff.

		P
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

		SP
		-> Optional. Sprite.
		-> Second token is the image, and it has to be there.
		-> After this token, and in any order, you can have "collectible" 
		and/or "collision". They are optional.
		-> After this line, each line of two tokens which are numbers are part 
		of this sprite. It's the XY position in double.
		-> If you find any other line, the processing of this sprite is done.

		M
		-> Optional. It's the path to the music file.
	*/
	free(map);
	return (0);
}

static int	process_json_lines(t_man *man, t_map *map)
{
	int	i;

	i = 0;
	while (map->pars.vars[i])
	{
		if (!strcmp(map->pars.vars[i][0], "JSON"))
		{
			if (!map->pars.vars[i][1]
				|| !update_image_array(man, map->pars.vars[i][1]))
				return (put_error(0, E_JSON, map->pars.vars[i][1], 0));
			free_arr((void **)map->pars.vars[i], free);
			remove_arr_elems((void **)map->pars.vars, i, i, 0);
		}
		else
			++i;
	}
	return (1);
}

/*
t_map	*create_map(t_man *man, const char *filepath)
{
	int	nbr_types;

	// ...
	nbr_types = 1;
	if (man->bonus)
		nbr_types = 9;
	(*map)->types = calloc(nbr_types + 1, sizeof(t_cell));
	if (!(*map)->types)
		exit_in_parsing(man, *map, E_FAIL_MEM, NULL);
	set_vec2(&(*map)->start_pos, -1, -1);
	(*map)->fog_color = get_color_rgba(0, 0, 0, 255);
	//
	parse_file_map(man, map, &line);
	return (map);
}

void	parse_file_map(t_man *man, t_map *map, char **line)
{
	int	n_texture;

	n_texture = 0;
	if (!(*line))
		exit_in_parsing(man, map, E_EMPTYMAP, NULL);
	if (!man->bonus)
	{
		find_cardinals_floor_ceiling(man, map, line, n_texture);
		map->map_walls = get_map_from_fd(man, map, line, &n_texture);
		valid_map(man, map);
		compose_skybox(man, map, NULL);
		fill_cells(man, map);
	}
	else
	{
		get_maps(man, map, line, &n_texture);
		maps_symetry(man, map);
		find_cardinals_floor_ceiling(man, map, line, n_texture);
		find_door_goal(man, map, line);
		find_skybox(man, map, line);
		find_portals(man, map, line);
		find_sprites(man, map, line);
		find_music(man, map, line);
		fill_cells(man, map);
	}
	compose_background(man, map);
	close(map->fd);
}
*/
