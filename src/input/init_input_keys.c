#include "cimmerian.h"

void	execute_input_action(t_man *man, int key, t_key_state new_state)
{
	int	i;

	i = 0;
	while (man->keys[i].key && man->keys[i].key != key)
		++i;
	if (!man->keys[i].key || !(man->keys[i].game_states & man->game_state))
		return ;
	if (man->keys[i].actions[new_state]
		&& (key == KEY_F11 || man->keys[i].state != new_state))
		man->keys[i].actions[new_state](man);
	man->keys[i].state = new_state;
	return ;
}

void	init_keys_wasdqe(t_man *man)
{
	man->keys[0].key = KEY_W;
	man->keys[0].game_states = GAME_STATE_PLAY;
	man->keys[0].actions[0] = action_move_y_decr;
	man->keys[0].actions[1] = action_move_y_incr;
	man->keys[1].key = KEY_A;
	man->keys[1].game_states = GAME_STATE_PLAY;
	man->keys[1].actions[0] = action_rotate_incr;
	man->keys[1].actions[1] = action_rotate_decr;
	man->keys[2].key = KEY_S;
	man->keys[2].game_states = GAME_STATE_PLAY;
	man->keys[2].actions[0] = action_move_y_incr;
	man->keys[2].actions[1] = action_move_y_decr;
	man->keys[3].key = KEY_D;
	man->keys[3].game_states = GAME_STATE_PLAY;
	man->keys[3].actions[0] = action_rotate_decr;
	man->keys[3].actions[1] = action_rotate_incr;
	man->keys[4].key = KEY_Q;
	man->keys[4].game_states = GAME_STATE_PLAY;
	man->keys[4].actions[0] = action_move_x_incr;
	man->keys[4].actions[1] = action_move_x_decr;
	man->keys[5].key = KEY_E;
	man->keys[5].game_states = GAME_STATE_PLAY;
	man->keys[5].actions[0] = action_move_x_decr;
	man->keys[5].actions[1] = action_move_x_incr;
	return ;
}

void	init_keys_arrows(t_man *man)
{
	man->keys[6].key = KEY_UP;
	man->keys[6].game_states = GAME_STATE_PLAY;
	man->keys[6].actions[0] = action_move_y_decr;
	man->keys[6].actions[1] = action_move_y_incr;
	man->keys[7].key = KEY_LEFT;
	man->keys[7].game_states = GAME_STATE_PLAY;
	man->keys[7].actions[0] = action_rotate_incr;
	man->keys[7].actions[1] = action_rotate_decr;
	man->keys[8].key = KEY_DOWN;
	man->keys[8].game_states = GAME_STATE_PLAY;
	man->keys[8].actions[0] = action_move_y_incr;
	man->keys[8].actions[1] = action_move_y_decr;
	man->keys[9].key = KEY_RIGHT;
	man->keys[9].game_states = GAME_STATE_PLAY;
	man->keys[9].actions[0] = action_rotate_decr;
	man->keys[9].actions[1] = action_rotate_incr;
	return ;
}

void	init_keys_misc(t_man *man)
{
	man->keys[10].key = KEY_SHIFT;
	man->keys[10].game_states = GAME_STATE_PLAY;
	man->keys[10].actions[0] = action_speed_walk;
	man->keys[10].actions[1] = action_speed_run;
	man->keys[11].key = KEY_F11;
	man->keys[11].game_states = GAME_STATE_ALL;
	man->keys[11].actions[1] = action_toggle_fullscreen;
	man->keys[12].key = KEY_F12;
	man->keys[12].game_states = GAME_STATE_ALL;
	man->keys[12].actions[1] = action_toggle_debug;
	man->keys[13].key = KEY_ESCAPE;
	man->keys[13].game_states = GAME_STATE_ALL;
	man->keys[13].actions[1] = action_close_window;
	return ;
}
