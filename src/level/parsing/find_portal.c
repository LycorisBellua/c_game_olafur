#include "cimmerian.h"

t_portal	*find_portal(t_map *map, int x, int y)
{
	int	i;

	i = 0;
	while (i < map->portal_len)
	{
		if (map->portals[i]->src_pos.x == x && map->portals[i]->src_pos.y == y)
			return (map->portals[i]);
		i++;
	}
	return (NULL);
}
