#include "cimmerian.h"

/*
	Set `dst_cardinal` to 0 if you want the destination to be the center of the 
	cell, instead of one of its edges. The map's default direction will be used.
*/

int	create_portal_array(t_man *man, t_map *map)
{
	t_img	*tex_portal;

	tex_portal = get_image(man, "portal_closed");
	// HARDCODED PORTALS - Use parsing instead
	if (!strcmp(map->filepath, "maps/0.map"))
	{
		map->portal_len = 2;
		map->portals = calloc(map->portal_len, sizeof(t_portal));
		if (!map->portals)
			return (0);

		/*
		map->portals[0].is_corridor = PORTAL_IS_CORRIDOR;
		map->portals[0].src_cardinal = 'S';
		set_ivec2(&map->portals[0].src_pos, 12, 0);
		map->portals[0].path_map = strdup("maps/1.map");
		map->portals[0].override_start_pos = 1;
		map->portals[0].dst_cardinal = 'E';
		set_ivec2(&map->portals[0].dst_pos, 0, 5);
		map->portals[0].tex = tex_portal;
		*/

		/*
		map->portals[1].is_corridor = PORTAL_IS_CORRIDOR;
		map->portals[1].src_cardinal = 'N';
		set_ivec2(&map->portals[1].src_pos, 15, 6);
		map->portals[1].path_map = strdup("maps/1.map");
		map->portals[1].override_start_pos = 1;
		map->portals[1].dst_cardinal = 'E';
		set_ivec2(&map->portals[1].dst_pos, 0, 5);
		map->portals[1].tex = tex_portal;
		*/
		map->portals[0].is_corridor = PORTAL_IS_CORRIDOR;
		map->portals[0].src_cardinal = 'S';
		set_ivec2(&map->portals[0].src_pos, 12, 0);
		map->portals[0].path_map = strdup("maps/0.map");
		map->portals[0].override_start_pos = 1;
		map->portals[0].dst_cardinal = 'N';
		set_ivec2(&map->portals[0].dst_pos, 12, 23);
		map->portals[0].tex = tex_portal;

		map->portals[1].is_corridor = PORTAL_IS_CORRIDOR;
		map->portals[1].src_cardinal = 'N';
		set_ivec2(&map->portals[1].src_pos, 12, 23);
		map->portals[1].path_map = strdup("maps/0.map");
		map->portals[1].override_start_pos = 1;
		map->portals[1].dst_cardinal = 'S';
		set_ivec2(&map->portals[1].dst_pos, 12, 0);
		map->portals[1].tex = tex_portal;
	}
	else if (!strcmp(map->filepath, "maps/1.map"))
	{
		map->portal_len = 2;
		map->portals = calloc(map->portal_len, sizeof(t_portal));
		if (!map->portals)
			return (0);

		map->portals[0].is_corridor = PORTAL_IS_CORRIDOR;
		map->portals[0].src_cardinal = 'E';
		set_ivec2(&map->portals[0].src_pos, 0, 5);
		map->portals[0].path_map = strdup("maps/0.map");
		map->portals[0].override_start_pos = 1;
		map->portals[0].dst_cardinal = 'S';
		set_ivec2(&map->portals[0].dst_pos, 12, 0);
		map->portals[0].tex = tex_portal;

		map->portals[1].is_corridor = PORTAL_IS_CORRIDOR;
		map->portals[1].src_cardinal = 'W';
		set_ivec2(&map->portals[1].src_pos, 6, 5);
		map->portals[1].path_map = strdup("maps/0.map");
		map->portals[1].override_start_pos = 1;
		map->portals[1].dst_cardinal = 'S';
		set_ivec2(&map->portals[1].dst_pos, 12, 0);
		map->portals[1].tex = tex_portal;
	}
	return (1);
}

t_portal	*find_portal(t_map *map, int x, int y)
{
	int	i;

	if (!map)
		return (0);
	i = 0;
	while (i < map->portal_len)
	{
		if (map->portals[i].src_pos.x == x && map->portals[i].src_pos.y == y)
			return (map->portals + i);
		++i;
	}
	return (0);
}
