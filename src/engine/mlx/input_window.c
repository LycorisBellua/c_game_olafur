#include "cimmerian.h"

int	close_window_callback(t_man *man)
{
	mlx_loop_end(man->mlx);
	return (0);
}

int	move_or_resize_window_callback(int x, int y, int width, int height,
	t_man *man)
{
	set_ivec2(&man->res.window_position, x, y);
	if (width != man->res.window_size.x || height != man->res.window_size.y)
	{
		set_ivec2(&man->res.window_size, width, height);
		set_viewport(man, man->res.window_size);
		free_frame(man);
		if (!init_frame(man))
			exit(EXIT_FAILURE);
		mlx_window_clear(man->mlx, man->window);
		display_frame(man);
	}
	return (0);
}
