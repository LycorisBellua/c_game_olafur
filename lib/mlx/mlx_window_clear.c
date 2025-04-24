#include "mlx_int.h"

int	mlx_window_clear(t_xvar *xvar, t_win_list *win)
{
	XClearWindow(xvar->display, win->window);
	if (xvar->do_flush)
		XFlush(xvar->display);
	return (0);
}
