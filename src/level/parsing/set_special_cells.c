#include "olafur.h"

static t_door	*find_door(t_map *map, t_ivec2 p);
static t_portal	*find_portal(t_map *map, t_ivec2 p);

int	set_special_cells(t_map *map, t_cell *c, t_ivec2 p)
{
	c->height = 1;
	c->is_empty_space = isspace(map->pars.map_wall[p.y][p.x]);
	c->door = find_door(map, p);
	c->portal = find_portal(map, p);
	if (map->pars.map_wall[p.y][p.x] == 'G')
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

static t_door	*find_door(t_map *map, t_ivec2 p)
{
	int	i;

	if (!map->doors)
		return (0);
	i = 0;
	while (map->doors[i])
	{
		if (map->doors[i]->pos.x == p.x && map->doors[i]->pos.y == p.y)
			return (map->doors[i]);
		++i;
	}
	return (0);
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
