#include "cimmerian.h"

void	toggle_fullscreen(t_man *man)
{
	static int	fullscreen;

	fullscreen = !fullscreen;
	mlx_window_fullscreen(man->xvar, fullscreen);
	return ;
}

t_ivec2	get_monitor_size(t_man *man)
{
	t_ivec2	size;

	mlx_screen_size(man->xvar, &size.x, &size.y);
	return (size);
}
