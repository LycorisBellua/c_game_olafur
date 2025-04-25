#include "mlx.h"

int	mlx_window_resize(t_xvar *xvar, t_win_list *win, int x, int y)
{
	XResizeWindow(xvar->display, win->window, x, y);
	if (xvar->do_flush)
		XFlush(xvar->display);
	return (0);
}
