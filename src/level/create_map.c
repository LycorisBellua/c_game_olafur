#include "cimmerian.h"

static int	set_map_cells(t_man *man, t_map *map);

t_map	*create_map(t_man *man, const char *filepath)
{
	t_img	*src_skybox;
	t_map	*map;

	map = calloc(1, sizeof(t_map));
	if (!map)
		put_error_and_exit(man, "", EXIT_FAILURE);
	map->filepath = strdup(filepath);
	if (!map->filepath)
	{
		free_map(map);
		put_error_and_exit(man, "", EXIT_FAILURE);
	}
	/*
		TODO:
		"Extract" functions are basic parsing
	*/
	if (!extract_maps_and_player_start(map) || !extract_sprites(man, map))
	{
		free_map(map);
		put_error_and_exit(man, "", EXIT_FAILURE);
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
	create_portal_array(man, map);
	if (!map->skybox || !map->background || !set_map_cells(man, map))
	{
		free_map(map);
		put_error_and_exit(man, "", EXIT_FAILURE);
	}
	return (map);
}

void	free_cells(t_map *map)
{
	int	i;

	if (!map || !map->cells)
		return ;
	i = 0;
	while (i < map->size.y)
	{
		free(map->cells[i]);
		++i;
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
			c->is_door = map->map_walls[i] == 'D';
			c->is_goal = map->map_walls[i] == 'G';
			c->is_obstacle = c->is_empty_space || c->is_door || c->is_goal
				|| (map->map_walls[i] > '0' && map->map_walls[i] <= '9');
			c->is_visible = c->is_obstacle && !c->is_empty_space;

			c->portal = find_portal(map, coord.x, coord.y);
			if (c->portal && !c->portal->is_corridor)
				c->is_visible = 1;

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
			if (c->is_door)
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
