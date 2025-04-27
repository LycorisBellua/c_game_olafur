#include "cimmerian.h"

int	key_press_callback(int keycode, t_man *man);
int	key_release_callback(int keycode, t_man *man);
int	mouse_press_callback(int button, int x, int y, t_man *man);
int	mouse_release_callback(int button, int x, int y, t_man *man);
int	mouse_move_callback(int x, int y, t_man *man);
int	focus_loss_callback(t_man *man);
int	move_or_resize_window_callback(int x, int y, int width, int height,
		t_man *man);

/*
	When the user clicks outside of the window (or on the window's header), the 
	app loses focus of the window. While the focus is lost, the window doesn't 
	catch events anymore.

	This is relevant when a key is being pressed, and there's focus loss before 
	the corresponding key release event can happen. The key release is lost, 
	making the player keep moving. It keeps moving while out of focus, and also 
	once it's in focus once more, and there's no way to make the player stop.

	This is where the cancel function comes in: when focus is lost, cancel all 
	input.
*/

void	init_input_handling(t_man *man)
{
	set_ivec2(&man->cursor, -1, -1);
	init_input_keys(man);
	mlx_hook(man->xvar, KeyPress, 1L << 0, key_press_callback, man);
	mlx_hook(man->xvar, KeyRelease, 1L << 1, key_release_callback, man);
	mlx_hook(man->xvar, ButtonPress, 1L << 2, mouse_press_callback, man);
	mlx_hook(man->xvar, ButtonRelease, 1L << 3, mouse_release_callback, man);
	mlx_hook(man->xvar, MotionNotify, 1L << 6, mouse_move_callback, man);
	mlx_hook(man->xvar, FocusOut, 1L << 21, cancel_keys_in_game_state, man);
	mlx_hook(man->xvar, ConfigureNotify, 1L << 17,
		move_or_resize_window_callback, man);
	mlx_loop_hook(man->xvar, game_loop, man);
	return ;
}
