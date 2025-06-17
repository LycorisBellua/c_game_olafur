#include "olafur.h"

static int	is_player_symbol(char c);

int	set_player_data(t_map *map)
{
	int	i;
	int	j;
	int	found_player;

	i = -1;
	found_player = 0;
	while (map->pars.map_wall[++i])
	{
		j = -1;
		while (map->pars.map_wall[i][++j])
		{
			if (is_player_symbol(map->pars.map_wall[i][j]))
			{
				if (found_player)
					return (put_error(0, E_DUP_PLAYER, 0, 0));
				found_player = 1;
				set_vec2(&map->start_pos, j + 0.5, i + 0.5);
				map->start_dir = get_dir_from_cardinal(
						map->pars.map_wall[i][j]);
			}
		}
	}
	if (!found_player)
		return (put_error(0, E_NO_PLAYER, 0, 0));
	return (1);
}

static int	is_player_symbol(char c)
{
	return (c == 'N' || c == 'S' || c == 'W' || c == 'E');
}
