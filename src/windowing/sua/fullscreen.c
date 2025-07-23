#include "olafur.h"

void	toggle_fullscreen(t_man *man)
{
	static int	fullscreen;

	fullscreen = !fullscreen;
	sua_window_fullscreen(man->xvar, fullscreen);
	return ;
}

void	set_monitor_size(t_man *man)
{
	t_ivec2	size;

	sua_screen_size(man->xvar, &size.x, &size.y);
	man->res.monitor_size = size;
	man->res.fullscreen.x = (size.x - man->res.monitor_size.x) / 2;
	man->res.fullscreen.y = (size.y - man->res.monitor_size.y) / 2;
	return ;
}
