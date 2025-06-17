#include "olafur.h"

t_map	*create_map(t_man *man, const char *filepath)
{
	t_map	*map;

	map = fetch_map_data(filepath);
	if (!map)
		return (0);
	else if (!process_img_assets(man, map) || !process_sound_assets(man, map)
		|| !process_wall_types(man, map) || !process_spec_wall_types(man, map)
		|| !process_floor_types(man, map) || !process_ceil_types(man, map)
		|| !process_background_image(man, map) || !process_portals(man, map)
		|| !process_sprites(man, map) || !is_var_list_empty(map)
		|| !check_map_symbols(map) || !set_player_data(map)
		|| !populate_cells(map))
	{
		free_map(map);
		return (0);
	}
	release_parsing_data(map);
	return (map);
}
