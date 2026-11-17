#include "olafur.h"

void	free_sprite_array(t_map *map)
{
	int	i;

	if (!map)
		return ;
	if (map->sprites)
	{
		i = 0;
		while (map->sprites[i])
		{
			free(map->sprites[i]);
			++i;
		}
		free(map->sprites);
	}
	map->sprites = 0;
	map->sprite_len = 0;
	return ;
}
