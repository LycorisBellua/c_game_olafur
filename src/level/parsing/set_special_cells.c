#include "olafur.h"

static t_portal	*find_portal(t_map *map, t_ivec2 p);

int	set_special_cells(t_map *map, t_cell *c, t_ivec2 p)
{
	c->height = 1;
	c->is_empty_space = isspace(map->pars.map_wall[p.y][p.x]);
	c->portal = find_portal(map, p);
	if (map->pars.map_wall[p.y][p.x] == 'D')
	{
		c->door = calloc(1, sizeof(t_door));
		if (!c->door)
			return (put_error(0, E_FAIL_MEM, 0, 0));
		c->door->m = map;
		set_ivec2(&c->door->pos, p.x, p.y);
		c->door->cardinal = 0;
		c->door->tex_closed = map->pars.tex_door_closed;
		c->door->tex_open = map->pars.tex_door_open;
		c->door->is_open = 0;
	}
	else if (map->pars.map_wall[p.y][p.x] == 'G')
	{
		c->is_goal = 1;
		c->height = 3;
		c->tex_north = map->pars.tex_goal;
		c->tex_south = map->pars.tex_goal;
		c->tex_west = map->pars.tex_goal;
		c->tex_east = map->pars.tex_goal;
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
