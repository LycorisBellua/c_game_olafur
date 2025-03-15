#include "cimmerian.h"

int	cross_window_button_callback(t_man *man);
int	mouse_callback(int button, int x, int y, t_man *man);
int	key_press_callback(int keycode, t_man *man);
int	key_release_callback(int keycode, t_man *man);

/*
	I had to remove `mlx_mouse_hide(man->mlx, man->window)` because it was 
	causing a leak.
*/
void	init_input_handling(t_man *man)
{
	mlx_hook(man->window, 17, 0L, cross_window_button_callback, man);
	mlx_hook(man->window, 4, 1L << 2, mouse_callback, man);
	mlx_hook(man->window, 2, 1L << 0, key_press_callback, man);
	mlx_hook(man->window, 3, 1L << 1, key_release_callback, man);
	mlx_loop_hook(man->mlx, game_loop, man);
	set_ivec2(&man->cursor, -1, -1);
	return ;
}

void	handle_input(t_man *man)
{
	//TODO
	/*
	MOVE CURSOR:
	mlx_mouse_get_pos(man->mlx, man->window, &pos.x, &pos.y);
	*/
	update_player_transform(man);
	return ;
}
