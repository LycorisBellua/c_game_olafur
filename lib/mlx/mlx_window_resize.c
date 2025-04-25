#include "mlx.h"

void	mlx_window_resize(t_xvar *xvar, int x, int y)
{
	XResizeWindow(xvar->display, xvar->window, x, y);
	if (xvar->do_flush)
		XFlush(xvar->display);
	return ;
}
