#include "cimmerian.h"

int	key_press_callback(int keycode, t_man *man);
int	key_release_callback(int keycode, t_man *man);
int	mouse_press_callback(int button, int x, int y, t_man *man);
int	mouse_release_callback(int button, int x, int y, t_man *man);
int	mouse_move_callback(int x, int y, t_man *man);
int	focus_loss_callback(t_man *man);
int	move_or_resize_window_callback(int x, int y, int width, int height,
		t_man *man);

void	init_input_handling(t_man *man)
{
	set_ivec2(&man->cursor, -1, -1);
	init_keys_wasdqe(man);
	init_keys_arrows(man);
	init_keys_misc(man);
	mlx_hook(man->xvar, 2, 1L << 0, key_press_callback, man);
	mlx_hook(man->xvar, 3, 1L << 1, key_release_callback, man);
	mlx_hook(man->xvar, 4, 1L << 2, mouse_press_callback, man);
	mlx_hook(man->xvar, 5, 1L << 3, mouse_release_callback, man);
	mlx_hook(man->xvar, 6, 1L << 6, mouse_move_callback, man);
	mlx_hook(man->xvar, 10, 1L << 21, focus_loss_callback, man);
	mlx_hook(man->xvar, 22, 1L << 17, move_or_resize_window_callback, man);
	mlx_loop_hook(man->xvar, game_loop, man);
	return ;
}
