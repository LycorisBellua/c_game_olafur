#include "mlx_int.h"

int	mlx_window_resize(t_xvar *xvar, t_win_list *win, int width, int height)
{
	XResizeWindow(xvar->display, win->window, width, height);
	if (xvar->do_flush)
		XFlush(xvar->display);
	return (0);
}
