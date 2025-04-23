#include "cimmerian.h"

void	free_frames(t_man *man);

void	toggle_fullscreen(t_man *man)
{
	static int	fullscreen;
	t_ivec2		new_size;

	fullscreen = !fullscreen;
	if (fullscreen)
		new_size = man->res.monitor_size;
	else
		new_size = man->res.window_size_default;
	mlx_resize_window(man->mlx, man->window, new_size.x, new_size.y);
	return ;
}

t_ivec2	get_monitor_size(t_man *man)
{
	t_ivec2	size;

	mlx_get_screen_size(man->mlx, &size.x, &size.y);
	return (size);
}
