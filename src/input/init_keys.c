#include "cimmerian.h"

/*
	By default, the window closing event is only triggered by the cross window 
	button or Alt+F4. We also add ESC.
*/

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

void	init_input_keys(t_man *man)
{
	const int	game_play = GAME_STATE_PLAY - 1;
	const int	game_success = GAME_STATE_SUCCESS - 1;
	const int	game_failure = GAME_STATE_FAILURE - 1;
	const int	key_pressed = KEY_STATE_PRESSED - 1;
	const int	key_held = KEY_STATE_HELD - 1;
	const int	key_released = KEY_STATE_RELEASED - 1;

	(void)key_released;

	man->keys[0].keycode = KEY_W;
	man->keys[0].actions[game_play][key_pressed] = action_move_forward;
	man->keys[0].actions[game_play][key_held] = action_move_forward;

	man->keys[1].keycode = KEY_A;
	man->keys[1].actions[game_play][key_pressed] = action_turn_left;
	man->keys[1].actions[game_play][key_held] = action_turn_left;

	man->keys[2].keycode = KEY_S;
	man->keys[2].actions[game_play][key_pressed] = action_move_backward;
	man->keys[2].actions[game_play][key_held] = action_move_backward;

	man->keys[3].keycode = KEY_D;
	man->keys[3].actions[game_play][key_pressed] = action_turn_right;
	man->keys[3].actions[game_play][key_held] = action_turn_right;

	man->keys[4].keycode = KEY_Q;
	man->keys[4].actions[game_play][key_pressed] = action_move_left;
	man->keys[4].actions[game_play][key_held] = action_move_left;

	man->keys[5].keycode = KEY_E;
	man->keys[5].actions[game_play][key_pressed] = action_move_right;
	man->keys[5].actions[game_play][key_held] = action_move_right;

	man->keys[6].keycode = KEY_UP;
	man->keys[6].actions[game_play][key_pressed] = action_move_forward;
	man->keys[6].actions[game_play][key_held] = action_move_forward;

	man->keys[7].keycode = KEY_LEFT;
	man->keys[7].actions[game_play][key_pressed] = action_turn_left;
	man->keys[7].actions[game_play][key_held] = action_turn_left;

	man->keys[8].keycode = KEY_DOWN;
	man->keys[8].actions[game_play][key_pressed] = action_move_backward;
	man->keys[8].actions[game_play][key_held] = action_move_backward;

	man->keys[9].keycode = KEY_RIGHT;
	man->keys[9].actions[game_play][key_pressed] = action_turn_right;
	man->keys[9].actions[game_play][key_held] = action_turn_right;

	man->keys[10].keycode = KEY_SHIFT;
	man->keys[10].actions[game_play][key_pressed] = action_run;
	man->keys[10].actions[game_play][key_held] = action_run;

	man->keys[11].keycode = KEY_F11;
	man->keys[11].actions[game_play][key_pressed] = action_toggle_fullscreen;
	man->keys[11].actions[game_success][key_pressed] = action_toggle_fullscreen;
	man->keys[11].actions[game_failure][key_pressed] = action_toggle_fullscreen;

	man->keys[12].keycode = KEY_F12;
	man->keys[12].actions[game_play][key_pressed] = action_toggle_debug;
	man->keys[12].actions[game_success][key_pressed] = action_toggle_debug;
	man->keys[12].actions[game_failure][key_pressed] = action_toggle_debug;

	man->keys[13].keycode = KEY_ESCAPE;
	man->keys[13].actions[game_play][key_pressed] = action_close_window;
	man->keys[13].actions[game_success][key_pressed] = action_close_window;
	man->keys[13].actions[game_failure][key_pressed] = action_close_window;

	man->keys[14].keycode = KEY_BACKSPACE;
	man->keys[14].actions[game_play][key_pressed] = action_move_to_start;
	man->keys[14].actions[game_success][key_pressed] = action_move_to_start;
	return ;
}
