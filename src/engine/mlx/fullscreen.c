#include "cimmerian.h"

void	free_frames(t_man *man);

void	toggle_fullscreen(t_man *man)
{
	static int	fullscreen;

	fullscreen = !fullscreen;
	mlx_window_fullscreen(man->mlx, man->window, fullscreen);
	return ;
}

t_ivec2	get_monitor_size(t_man *man)
{
	t_ivec2	size;

	mlx_get_screen_size(man->mlx, &size.x, &size.y);
	return (size);
}
