#include "cimmerian.h"

static int		allocate_cell_array(t_map *map);
static t_portal	*find_portal(t_map *map, t_ivec2 p);

int	populate_cells(t_map *map)
{
	/*
		TODO:
		- map->size is big enough for any three map to fit in.
		- Allocate and set the cells.
		- In a floor or ceiling map, '0' means "show the background".
	*/
	if (!allocate_cell_array(map))
		return (0);
	return (0);
}

static int	allocate_cell_array(t_map *map)
{
	int	i;

	// Why is it not `sizeof(t_cell *)`?
	map->cells = calloc(map->size.y, sizeof(t_cell));
	if (!map->cells)
		return (put_error(0, E_FAIL_MEM, 0, 0));
	i = 0;
	while (i < map->size.y)
	{
		map->cells[i] = calloc(map->size.x, sizeof(t_cell));
		if (!map->cells[i])
			return (put_error(0, E_FAIL_MEM, 0, 0));
		++i;
	}
	return (1);
}

static t_portal	*find_portal(t_map *map, t_ivec2 p)
{
	int	i;

	if (!map->portals)
		return (0);
	i = 0;
	while (map->portals[i])
	{
		if (map->portals[i]->src_pos.x == p.x
			&& map->portals[i]->src_pos.y == p.y)
			return (map->portals[i]);
		++i;
	}
	return (0);
}

//------------------------------------------------------------------------------

/*
static int	old_function(t_map *map)
{
	t_ivec2	p;
	t_cell	*c;
	int		i;

	p.y = -1;
	while (++p.y < map->size.y)
	{
		p.x = -1;
		while (++p.x < map->size.x)
		{
			c = &map->cells[p.y][p.x];
			c->is_empty_space = isspace(map->pars.map_wall[p.y][p.x])
				|| !map->pars.map_wall[p.y][p.x];
			if (map->pars.map_wall[p.y][p.x] == 'D')
			{
				c->door = calloc(1, sizeof(t_door));
				if (!c->door)
					return (put_error(0, E_FAIL_MEM, 0, 0));
			}
			c->is_goal = map->pars.map_wall[p.y][p.x] == 'G';
			c->is_obstacle = c->is_empty_space || c->door || c->is_goal
				|| (isdigit(map->pars.map_wall[p.y][p.x])
					&& map->pars.map_wall[p.y][p.x] != '0');
			c->portal = find_portal(map, p);
			c->is_visible = (c->is_obstacle && !c->is_empty_space)
				|| is_portal_visible(c->portal);
			c->height = 1;
			if (c->is_goal)
				c->height = 3;
			if (map->pars.map_floor)
			{
				i = 0;
				while (map->pars.tex_types_floor[i].symbol)
				{
					if (map->pars.tex_types_floor[i].symbol == map->pars.map_floor[p.y][p.x])
					{
						c->tex_floor = map->pars.tex_types_floor[i].tex;
						break ;
					}
					++i;
				}
			}
			if (map->pars.map_ceil)
			{
				i = 0;
				while (map->pars.tex_types_ceil[i].symbol)
				{
					if (map->pars.tex_types_ceil[i].symbol == map->pars.map_ceil[p.y][p.x])
					{
						c->tex_ceiling = map->pars.tex_types_ceil[i].tex;
						break ;
					}
					++i;
				}
			}
			if (c->door)
			{
				c->door->m = map;
				set_ivec2(&c->door->pos, p.x, p.y);
				c->door->cardinal = 0;
				c->door->tex_closed = map->pars.tex_door_closed;
				c->door->tex_open = map->pars.tex_door_open;
				c->door->is_open = 0;
			}
			else if (c->is_goal)
			{
				c->tex_north = map->pars.tex_goal;
				c->tex_south = map->pars.tex_goal;
				c->tex_west = map->pars.tex_goal;
				c->tex_east = map->pars.tex_goal;
			}
			else
			{
				i = 0;
				while (map->pars.tex_types_wall[i].symbol)
				{
					if (map->pars.tex_types_wall[i].symbol == map->pars.map_wall[p.y][p.x])
					{
						c->tex_north = map->pars.tex_types_wall[i].tex_north;
						c->tex_south = map->pars.tex_types_wall[i].tex_south;
						c->tex_west = map->pars.tex_types_wall[i].tex_west;
						c->tex_east = map->pars.tex_types_wall[i].tex_east;
						break ;
					}
					++i;
				}
			}
		}
	}
	return (1);
}
*/
