#include "mlx.h"

void	mlx_window_destroy(t_xvar *xvar)
{
	if (!xvar->window)
		return ;
	XDestroyWindow(xvar->display, xvar->window);
	xvar->window = 0;
	XFreeGC(xvar->display, xvar->gc);
	xvar->gc = (void *)0;
	if (xvar->do_flush)
		XFlush(xvar->display);
	return ;
}
