#include "cimmerian.h"

int	key_press_callback(int keycode, t_man *man);
int	key_release_callback(int keycode, t_man *man);
int	mouse_press_callback(int button, int x, int y, t_man *man);
int	mouse_release_callback(int button, int x, int y, t_man *man);
int	mouse_move_callback(int x, int y, t_man *man);
int	cross_window_button_callback(t_man *man);

/*
	`mlx_hook(man->window, 6, 1L << 6, mouse_move_callback, man);`
	This hook breaks when "resizing" the window (destroying and recreating).
	I poll the mouse position in the main loop instead.
*/

void	init_input_handling(t_man *man)
{
	mlx_hook(man->window, 2, 1L << 0, key_press_callback, man);
	mlx_hook(man->window, 3, 1L << 1, key_release_callback, man);
	mlx_hook(man->window, 4, 1L << 2, mouse_press_callback, man);
	mlx_hook(man->window, 5, 1L << 3, mouse_release_callback, man);
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
