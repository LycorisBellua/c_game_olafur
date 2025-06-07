#include "cimmerian.h"

/*
static void	init_cells(t_man *man, t_map *map);
static void	set_cell_tex(t_cell *cell, t_img *img, t_tex_type *types);
static void	fill_cell(t_map *map, t_cell *c, int x, int y);

void	fill_cells(t_man *man, t_map *map)
{
	t_ivec2	p;
	t_cell	*c;

	p.y = -1;
	init_cells(man, map);
	while (++p.y < map->size.y)
	{
		p.x = -1;
		while (++p.x < map->size.x)
		{
			c = &map->cells[p.y][p.x];
			c->is_empty_space = isspace(map->map_walls[p.y][p.x])
				|| map->map_walls[p.y][p.x] == '\0';
			if (map->map_walls[p.y][p.x] == 'D')
				c->door = calloc(1, sizeof(t_door));
			c->is_goal = map->map_walls[p.y][p.x] == 'G';
			c->is_obstacle = c->is_empty_space || c->door || c->is_goal
				|| isvalid(map->map_walls[p.y][p.x], "12345678");
			c->portal = find_portal(map, p.x, p.y);
			c->is_visible = (c->is_obstacle && !c->is_empty_space)
				|| is_portal_visible(c->portal);
			c->height = powf(3.0, (float)c->is_goal);
			fill_cell(map, c, p.x, p.y);
		}
	}
}

static void	init_cells(t_man *man, t_map *map)
{
	int	y;

	y = 0;
	map->cells = calloc(map->size.y, sizeof(t_cell));
	if (!map->cells)
		exit_in_parsing(man, map, E_FAIL_MEM, NULL);
	while (y < map->size.y)
	{
		map->cells[y] = calloc(map->size.x, sizeof(t_cell));
		if (!map->cells[y])
			exit_in_parsing(man, map, E_FAIL_MEM, NULL);
		y++;
	}
}

static void	set_cell_tex(t_cell *cell, t_img *img, t_tex_type *types)
{
	if (!cell || (!img && !types))
		return ;
	else if (img)
	{
		cell->tex_north = img;
		cell->tex_south = img;
		cell->tex_west = img;
		cell->tex_east = img;
	}
	else
	{
		cell->tex_north = types->tex_north;
		cell->tex_south = types->tex_south;
		cell->tex_west = types->tex_west;
		cell->tex_east = types->tex_east;
	}
}

static void	fill_cell(t_map *map, t_cell *c, int x, int y)
{
	if (map->map_ceil_floor)
	{
		if (map->map_ceil_floor[y][x] == '1' && map->types[1].tex_f_indoor)
			c->tex_floor = map->types[1].tex_f_indoor;
		else if (map->map_ceil_floor[y][x] == '0'
			|| map->map_ceil_floor[y][x] == '1')
			c->tex_floor = map->types[1].tex_floor;
		if (map->map_ceil_floor[y][x] == '1')
			c->tex_ceiling = map->types[1].tex_ceiling;
	}
	if (c->door)
	{
		c->door->m = map;
		set_ivec2(&c->door->pos, x, y);
		c->door->cardinal = 0;
		c->door->tex_closed = map->types[1].tex_door_closed;
		c->door->tex_open = map->types[1].tex_door_open;
		c->door->is_open = 0;
	}
	else if (c->is_goal)
		set_cell_tex(c, map->types[1].tex_goal, NULL);
	else if (isdigit(map->map_walls[y][x]))
		set_cell_tex(c, NULL, &(map->types[map->map_walls[y][x] - '0']));
	else
		set_cell_tex(c, NULL, &(map->types[0]));
}
*/
