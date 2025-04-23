#include "cimmerian.h"

int		set_swap_buf_frames(t_man *man);
void	free_swap_buf_frames(t_man *man);

int	cross_window_button_callback(t_man *man)
{
	mlx_loop_end(man->mlx);
	return (0);
}

int	move_or_resize_window_callback(int x, int y, int width, int height,
	t_man *man)
{
	(void)x;
	(void)y;
	if (width != man->res.window_size.x || height != man->res.window_size.y)
	{
		set_ivec2(&man->res.window_size, width, height);
		set_viewport(man, man->res.window_size);
		free_swap_buf_frames(man);
		set_swap_buf_frames(man);
		mlx_clear_window(man->mlx, man->window);
	}
    return (0);
}
