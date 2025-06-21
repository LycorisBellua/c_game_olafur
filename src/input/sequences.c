#include "olafur.h"

static void	easter_egg(t_man *man);
static void	transport_player_to_map(t_man *man, int map_index);

void	konami_code(t_man *man, int pressed_key)
{
	static int	sequence[10];
	static int	index;

	if (!sequence[0])
	{
		sequence[0] = KEY_UP;
		sequence[1] = KEY_UP;
		sequence[2] = KEY_DOWN;
		sequence[3] = KEY_DOWN;
		sequence[4] = KEY_LEFT;
		sequence[5] = KEY_RIGHT;
		sequence[6] = KEY_LEFT;
		sequence[7] = KEY_RIGHT;
		sequence[8] = KEY_B;
		sequence[9] = KEY_A;
	}
	if (pressed_key != sequence[index++])
		index = 0;
	if (index == sizeof(sequence) / sizeof(int))
	{
		index = 0;
		easter_egg(man);
	}
	return ;
}

/*
	If in the elevator, go to the secret elevator.
	If on the secret elevator, go back to the previous elevator.
*/
static void	easter_egg(t_man *man)
{
	static int	index_egg = -1;
	static int	index_curr = -1;
	const char	*name_egg;
	const char	*name_curr;

	name_egg = get_filename(MAP_EASTER_EGG);
	name_curr = get_filename(man->maps[man->curr_map]->filepath);
	if (!name_egg || !name_curr || strncmp(name_egg, "elev", 4)
		|| strncmp(name_curr, "elev", 4) || !strncmp(name_curr, "elev4", 5))
		return ;
	if (index_egg < 0)
		index_egg = add_map(man, MAP_EASTER_EGG);
	if (index_egg < 0)
		return ;
	if (index_curr < 0)
		index_curr = man->curr_map;
	if (man->curr_map != index_egg)
		transport_player_to_map(man, index_egg);
	else
	{
		transport_player_to_map(man, index_curr);
		index_curr = -1;
	}
	return ;
}

static void	transport_player_to_map(t_man *man, int map_index)
{
	t_map	*map;

	man->curr_map = map_index;
	map = man->maps[man->curr_map];
	if ((int)man->player.pos.x >= map->size.x
		|| (int)man->player.pos.y >= map->size.y)
		set_player_transform(man, map->start_pos, man->player.dir);
	return ;
}
