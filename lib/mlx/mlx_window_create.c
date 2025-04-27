#include "mlx.h"

static void	mlx_int_resize_win(t_xvar *xvar, int w, int h);
static void	mlx_int_wait_first_expose(t_xvar *xvar);

/*
	We do not use White/BlackPixel macro, TrueColor Visual make sure 0 is black 
	and -1 is white.
*/
int	mlx_window_create(t_xvar *xvar, int width, int height, char *title)
{
	XSetWindowAttributes	xswa;
	XGCValues				xgcv;

	if (xvar->window)
		mlx_window_destroy(xvar);
	xswa.background_pixel = 0;
	xswa.border_pixel = -1;
	xswa.colormap = xvar->cmap;
	xswa.event_mask = 0xFFFFFF;
	xvar->window = XCreateWindow(xvar->display, xvar->root, 0, 0, width,
			height, 0, CopyFromParent, InputOutput, xvar->visual,
			CWEventMask | CWBackPixel | CWBorderPixel | CWColormap, &xswa);
	if (!xvar->window)
		return (-1);
	mlx_int_resize_win(xvar, width, height);
	XStoreName(xvar->display, xvar->window, title);
	XSetWMProtocols(xvar->display, xvar->window, &(xvar->wm_delete_window), 1);
	xgcv.foreground = -1;
	xgcv.function = GXcopy;
	xgcv.plane_mask = AllPlanes;
	xvar->gc = XCreateGC(xvar->display, xvar->window,
			GCFunction | GCPlaneMask | GCForeground, &xgcv);
	XMapRaised(xvar->display, xvar->window);
	mlx_int_wait_first_expose(xvar);
	return (0);
}

static void	mlx_int_resize_win(t_xvar *xvar, int w, int h)
{
	XSizeHints	hints;
	int			screen_x;
	int			screen_y;
	long		hints_mask;

	mlx_screen_size(xvar, &screen_x, &screen_y);
	XGetWMNormalHints(xvar->display, xvar->window, &hints, &hints_mask);
	hints.width = w;
	hints.height = h;
	hints.min_width = w;
	hints.min_height = h;
	hints.max_width = screen_x;
	hints.max_height = screen_y;
	hints.flags = PPosition | PSize | PMinSize | PMaxSize;
	XSetWMNormalHints(xvar->display, xvar->window, &hints);
	return ;
}

static void	mlx_int_wait_first_expose(t_xvar *xvar)
{
	XEvent	ev;

	XWindowEvent(xvar->display, xvar->window, ExposureMask, &ev);
	XPutBackEvent(xvar->display, &ev);
	return ;
}
