#include "olafur.h"

static void	init_system_keys(t_key *k);
static void	set_key(t_key *key, int keycode, t_action pressed, t_action held);
static void	set_key_in_all_states(t_key *key, int keycode, t_action pressed);

/*
	By default, the window closing event is only triggered by the cross window 
	button or Alt+F4. We also add ESC.
*/
void	init_input_keys(t_man *man)
{
	t_key	*k;

	k = man->keys;
	set_key(&k[0], GLFW_KEY_W, action_move_forward, action_move_forward);
	set_key(&k[1], GLFW_KEY_A, action_turn_left, action_turn_left);
	set_key(&k[2], GLFW_KEY_S, action_move_backward, action_move_backward);
	set_key(&k[3], GLFW_KEY_D, action_turn_right, action_turn_right);
	set_key(&k[4], GLFW_KEY_Q, action_move_left, action_move_left);
	set_key(&k[5], GLFW_KEY_E, action_move_right, action_move_right);
	set_key(&k[6], GLFW_KEY_UP, action_move_forward, action_move_forward);
	set_key(&k[7], GLFW_KEY_LEFT, action_turn_left, action_turn_left);
	set_key(&k[8], GLFW_KEY_DOWN, action_move_backward, action_move_backward);
	set_key(&k[9], GLFW_KEY_RIGHT, action_turn_right, action_turn_right);
	set_key(&k[10], GLFW_KEY_LEFT_SHIFT, action_run, action_run);
	set_key(&k[11], GLFW_KEY_RIGHT_SHIFT, action_run, action_run);
	init_system_keys(k);
	set_key(&k[17], GLFW_KEY_PAGE_UP, action_move_forward, action_move_forward);
	set_key(&k[18], GLFW_KEY_PAGE_DOWN, action_move_backward,
		action_move_backward);
	return ;
}

/* Keys that aren't about movement: F5, F10, F11, F12, ESC. */
static void	init_system_keys(t_key *k)
{
	set_key(&k[12], GLFW_KEY_F5, action_move_to_start, 0);
	k[12].actions[GAME_STATE_SUCCESS - 1][KEY_STATE_PRESSED - 1]
		= action_move_to_start;
	set_key_in_all_states(&k[13], GLFW_KEY_F10, action_toggle_vsync);
	set_key_in_all_states(&k[14], GLFW_KEY_F11, action_toggle_fullscreen);
	set_key_in_all_states(&k[15], GLFW_KEY_F12, action_toggle_debug);
	set_key_in_all_states(&k[16], GLFW_KEY_ESCAPE, action_close_window);
	return ;
}

static void	set_key(t_key *key, int keycode, t_action pressed, t_action held)
{
	key->keycode = keycode;
	key->actions[GAME_STATE_PLAY - 1][KEY_STATE_PRESSED - 1] = pressed;
	key->actions[GAME_STATE_PLAY - 1][KEY_STATE_HELD - 1] = held;
	return ;
}

static void	set_key_in_all_states(t_key *key, int keycode, t_action pressed)
{
	key->keycode = keycode;
	key->actions[GAME_STATE_PLAY - 1][KEY_STATE_PRESSED - 1] = pressed;
	key->actions[GAME_STATE_SUCCESS - 1][KEY_STATE_PRESSED - 1] = pressed;
	key->actions[GAME_STATE_FAILURE - 1][KEY_STATE_PRESSED - 1] = pressed;
	return ;
}
