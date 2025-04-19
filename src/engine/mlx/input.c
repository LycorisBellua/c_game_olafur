#include "cimmerian.h"

int	key_press_callback(int keycode, t_man *man);
int	key_release_callback(int keycode, t_man *man);
int	mouse_press_callback(int button, int x, int y, t_man *man);
int	mouse_release_callback(int button, int x, int y, t_man *man);
int	mouse_move_callback(int x, int y, t_man *man);
int	cross_window_button_callback(t_man *man);

static void	init_wasd_and_arrow_keys(t_man *man);
static void	init_special_keys(t_man *man);

/*
	`mlx_hook(man->window, 6, 1L << 6, mouse_move_callback, man);`
	This hook breaks when "resizing" the window (destroying and recreating).
	I poll the mouse position in the main loop instead.
*/

void	init_input_handling(t_man *man)
{
	init_wasd_and_arrow_keys(man);
	init_special_keys(man);
	mlx_hook(man->window, 2, 1L << 0, key_press_callback, man);
	mlx_hook(man->window, 3, 1L << 1, key_release_callback, man);
	mlx_hook(man->window, 4, 1L << 2, mouse_press_callback, man);
	mlx_hook(man->window, 5, 1L << 3, mouse_release_callback, man);
	mlx_hook(man->window, 10, 1L << 21, focus_loss_callback, man);
	mlx_hook(man->window, 17, 0L, cross_window_button_callback, man);
	mlx_loop_hook(man->mlx, game_loop, man);
	return ;
}

void	poll_input_events(t_man *man)
{
	t_ivec2	pos;

	mlx_mouse_get_pos(man->mlx, man->window, &pos.x, &pos.y);
	mouse_move_callback(pos.x, pos.y, man);
	return ;
}

static void	init_wasd_and_arrow_keys(t_man *man)
{
	man->keys[0].key = KEY_W;
	man->keys[0].game_states = GAME_STATE_PLAY;
	man->keys[0].actions[0] = action_move_y_decr;
	man->keys[0].actions[1] = action_move_y_incr;
	man->keys[1].key = KEY_A;
	man->keys[1].game_states = GAME_STATE_PLAY;
	man->keys[1].actions[0] = action_move_x_incr;
	man->keys[1].actions[1] = action_move_x_decr;
	man->keys[2].key = KEY_S;
	man->keys[2].game_states = GAME_STATE_PLAY;
	man->keys[2].actions[0] = action_move_y_incr;
	man->keys[2].actions[1] = action_move_y_decr;
	man->keys[3].key = KEY_D;
	man->keys[3].game_states = GAME_STATE_PLAY;
	man->keys[3].actions[0] = action_move_x_decr;
	man->keys[3].actions[1] = action_move_x_incr;
	man->keys[4].key = KEY_LEFT;
	man->keys[4].game_states = GAME_STATE_PLAY;
	man->keys[4].actions[0] = action_rotate_incr;
	man->keys[4].actions[1] = action_rotate_decr;
	man->keys[5].key = KEY_RIGHT;
	man->keys[5].game_states = GAME_STATE_PLAY;
	man->keys[5].actions[0] = action_rotate_decr;
	man->keys[5].actions[1] = action_rotate_incr;
	return ;
}

static void	init_special_keys(t_man *man)
{
	man->keys[6].key = KEY_SHIFT;
	man->keys[6].game_states = GAME_STATE_PLAY;
	man->keys[6].actions[0] = action_speed_walk;
	man->keys[6].actions[1] = action_speed_run;
	man->keys[7].key = KEY_F11;
	man->keys[7].game_states = GAME_STATE_ALL;
	man->keys[7].actions[1] = action_toggle_fullscreen;
	man->keys[8].key = KEY_F12;
	man->keys[8].game_states = GAME_STATE_ALL;
	man->keys[8].actions[1] = action_toggle_debug;
	man->keys[9].key = KEY_ESCAPE;
	man->keys[9].game_states = GAME_STATE_ALL;
	man->keys[9].actions[1] = action_close_window;
	return ;
}
