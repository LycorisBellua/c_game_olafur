#include "cimmerian.h"

static int	set_map_cells(t_man *man, t_map *map);

t_map	*create_map(t_man *man, const char *filepath)
{
	t_img	*src_skybox;
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
	/*
		TODO:
		"Extract" functions are basic parsing
	*/
	if (!extract_maps_and_player_start(map) || !extract_sprites(man, map))
	{
		free_map(map);
		return (0);
	}
	map->fog_color = get_color_rgba(0, 0, 0, 255);
	// If 'C' and/or 'F' colors are specified in the file:
	//map->ceiling_color = get_color_rgba(255, 30, 30, 255);
	map->floor_color = get_color_rgba(42, 30, 30, 255);
	//
	/* `src_skybox` can be set to NULL if no skybox: */
	src_skybox = get_image(man, "src_skybox");
	if (src_skybox)
		map->fog_color = src_skybox->average_color[0];
	compose_skybox(man, map, src_skybox);
	compose_background(man, map);
	if (!map->skybox || !map->background || !create_portal_array(man, map)
		|| !set_map_cells(man, map))
	{
		free_map(map);
		return (0);
	}
	return (map);
}

void	free_cells(t_map *map)
{
	t_ivec2	coord;

	if (!map || !map->cells)
		return ;
	coord.y = 0;
	while (coord.y < map->size.y)
	{
		coord.x = 0;
		while (coord.x < map->size.x)
			free(map->cells[coord.y][coord.x++].door);
		free(map->cells[coord.y]);
		++coord.y;
	}
	free(map->cells);
	map->cells = 0;
	return ;
}

static int	set_map_cells(t_man *man, t_map *map)
{
	t_ivec2	coord;
	int		i;
	t_cell	*c;
	t_img	*img;
	t_img	*img_arr[7];

	img_arr[0] = 0;
	img_arr[1] = get_image(man, "tex_wall_01");
	img_arr[2] = get_image(man, "tex_wall_02");
	img_arr[3] = get_image(man, "tex_wall_03");
	img_arr[4] = get_image(man, "tex_wall_04");
	img_arr[5] = get_image(man, "tex_doors");
	img_arr[6] = get_image(man, "solid_color");
	map->cells = calloc(map->size.y, sizeof(t_cell *));
	if (!map->cells)
		return (0);
	i = 0;
	while (i < map->size.y)
	{
		map->cells[i] = calloc(map->size.x, sizeof(t_cell));
		if (!map->cells[i])
		{
			free_cells(map);
			return (0);
		}
		++i;
	}
	coord.y = 0;
	while (coord.y < map->size.y)
	{
		coord.x = 0;
		while (coord.x < map->size.x)
		{
			i = coord.y * map->size.x + coord.x;
			c = &map->cells[coord.y][coord.x];
			c->is_empty_space = isspace(map->map_walls[i]);
			if (map->map_walls[i] == 'D')
			{
				c->door = calloc(1, sizeof(t_door));
				if (!c->door)
				{
					free_cells(map);
					return (0);
				}
				c->door->m = map;
				c->door->pos = coord;
				c->door->cardinal = 0;
				c->door->tex_closed = img_arr[5];
				c->door->tex_open = 0;
				c->door->is_open = 0;
			}
			c->portal = find_portal(map, coord.x, coord.y);
			c->is_goal = map->map_walls[i] == 'G';
			c->is_obstacle = c->is_empty_space || c->door || c->is_goal
				|| (map->map_walls[i] > '0' && map->map_walls[i] <= '9');
			c->is_visible = (c->is_obstacle && !c->is_empty_space)
				|| (c->portal
					&& (c->portal->tex_closed || c->portal->tex_open));

			if (c->is_visible)
				c->height = 1.0;
			if (c->is_goal)
				c->height = 3.0;

			// IF MAP "CEIL-FLOOR"
			if (map->map_ceil_floor[i] == '0')
				c->tex_floor = get_image(man, "tex_floor");
			else if (map->map_ceil_floor[i] == '1')
			{
				c->tex_floor = get_image(man, "tex_floor_indoors");
				c->tex_ceiling = get_image(man, "tex_ceiling");
			}
			//
			if (c->door)
				img = img_arr[5];
			else if (c->is_goal)
				img = img_arr[6];
			else if (isdigit(map->map_walls[i]) && map->map_walls[i] < '5')
				img = img_arr[map->map_walls[i] - '0'];
			else
				img = img_arr[0];
			c->tex_north = img;
			c->tex_east = img;
			c->tex_south = img;
			c->tex_west = img;
			++coord.x;
		}
		++coord.y;
	}
	return (1);
}
