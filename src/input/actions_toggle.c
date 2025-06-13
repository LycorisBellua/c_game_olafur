#include "cimmerian.h"

void	action_toggle_fullscreen(t_man *man, int set)
{
	if (set)
		toggle_fullscreen(man);
	return ;
}

void	action_toggle_debug(t_man *man, int set)
{
	if (set)
		man->show_debug = !man->show_debug;
	return ;
}

void	action_close_window(t_man *man, int set)
{
	if (set)
		close_window_callback(man);
	return ;
}

void	action_move_to_start(t_man *man, int set)
{
	if (set)
	{
		change_game_state(man, GAME_STATE_PLAY);
		man->curr_map = 0;
		reset_player_transform(man);
	}
	return ;
}
