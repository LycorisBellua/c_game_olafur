#include "cimmerian.h"

static int	allocate_cell_array(t_map *map);
static void	set_tex_for_wall(t_map *map, t_cell *c, t_ivec2 p);
static void	set_tex_for_floor(t_map *map, t_cell *c, t_ivec2 p);
static void	set_tex_for_ceil(t_map *map, t_cell *c, t_ivec2 p);

int	populate_cells(t_map *map)
{
	t_ivec2	p;
	t_cell	*c;

	if (!allocate_cell_array(map))
		return (0);
	p.y = -1;
	while (++p.y < map->size.y)
	{
		p.x = -1;
		while (++p.x < map->size.x)
		{
			c = &map->cells[p.y][p.x];
			if (!set_special_cells(map, c, p))
				return (0);
			c->is_obstacle = c->is_empty_space || c->door || c->is_goal
				|| (isdigit(map->pars.map_wall[p.y][p.x])
					&& map->pars.map_wall[p.y][p.x] != '0');
			c->is_visible = (c->is_obstacle && !c->is_empty_space)
				|| is_portal_visible(c->portal);
			set_tex_for_wall(map, c, p);
			set_tex_for_floor(map, c, p);
			set_tex_for_ceil(map, c, p);
		}
	}
	return (1);
}

static int	allocate_cell_array(t_map *map)
{
	int	i;

	map->cells = calloc(map->size.y, sizeof(t_cell *));
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

static void	set_tex_for_wall(t_map *map, t_cell *c, t_ivec2 p)
{
	int	i;

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
	return ;
}

static void	set_tex_for_floor(t_map *map, t_cell *c, t_ivec2 p)
{
	int	i;

	if (!map->pars.map_floor)
		return ;
	i = 0;
	while (map->pars.tex_types_floor[i].symbol)
	{
		if (map->pars.tex_types_floor[i].symbol
			== map->pars.map_floor[p.y][p.x])
		{
			c->tex_floor = map->pars.tex_types_floor[i].tex;
			break ;
		}
		++i;
	}
	return ;
}

static void	set_tex_for_ceil(t_map *map, t_cell *c, t_ivec2 p)
{
	int	i;

	if (!map->pars.map_ceil)
		return ;
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
	return ;
}
