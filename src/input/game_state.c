#include "olafur.h"

void	cancel_keys_in_game_state(t_man *man)
{
	int	i;

	i = 0;
	while (man->keys[i].keycode)
	{
		if (man->keys[i].state != KEY_STATE_NONE)
			execute_input_action(man, man->keys[i].keycode, KEY_STATE_NONE);
		++i;
	}
	return ;
}

void	change_game_state(t_man *man, t_game_state new_state)
{
	cancel_keys_in_game_state(man);
	if (man->game_state != new_state)
	{
		if (new_state == GAME_STATE_SUCCESS)
			audio_source_play(man->audio.sources[SOUND_SUCCESS]);
		else if (new_state == GAME_STATE_FAILURE)
			audio_source_play(man->audio.sources[SOUND_FAILURE]);
	}
	man->game_state = new_state;
	return ;
}
