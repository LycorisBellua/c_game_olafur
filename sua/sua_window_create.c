#include "sua.h"

static void	sua_int_resize_win(t_xvar *xvar, int screen_w, int screen_h, int w,
				int h);
static void	sua_int_wait_first_expose(t_xvar *xvar);

/*
	We do not use White/BlackPixel macro, TrueColor Visual make sure 0 is black 
	and -1 is white.
*/
int	sua_window_create(t_xvar *xvar, int width, int height, char *title)
{
	XSetWindowAttributes	xswa;
	XGCValues				xgcv;
	int						screen_w;
	int						screen_h;

	if (xvar->window)
		sua_window_destroy(xvar);
	sua_screen_size(xvar, &screen_w, &screen_h);
	xswa.background_pixel = 0;
	xswa.border_pixel = -1;
	xswa.colormap = xvar->cmap;
	xswa.event_mask = 0xFFFFFF;
	xvar->window = XCreateWindow(xvar->display, xvar->root,
			screen_w / 2 - width / 2, screen_h / 2 - height / 2, width, height,
			0, CopyFromParent, InputOutput, xvar->visual,
			CWEventMask | CWBackPixel | CWBorderPixel | CWColormap, &xswa);
	if (!xvar->window)
		return (-1);
	sua_int_resize_win(xvar, screen_w, screen_h, width, height);
	XStoreName(xvar->display, xvar->window, title);
	XSetWMProtocols(xvar->display, xvar->window, &(xvar->wm_delete_window), 1);
	xgcv.foreground = -1;
	xgcv.function = GXcopy;
	xgcv.plane_mask = AllPlanes;
	xvar->gc = XCreateGC(xvar->display, xvar->window,
			GCFunction | GCPlaneMask | GCForeground, &xgcv);
	XMapRaised(xvar->display, xvar->window);
	sua_int_wait_first_expose(xvar);
	return (0);
}

static void	sua_int_resize_win(t_xvar *xvar, int screen_w, int screen_h, int w,
	int h)
{
	XSizeHints	hints;
	long		hints_mask;

	XGetWMNormalHints(xvar->display, xvar->window, &hints, &hints_mask);
	hints.width = w;
	hints.height = h;
	hints.min_width = w;
	hints.min_height = h;
	hints.max_width = screen_w;
	hints.max_height = screen_h;
	hints.flags = PPosition | PSize | PMinSize | PMaxSize;
	XSetWMNormalHints(xvar->display, xvar->window, &hints);
	return ;
}

static void	sua_int_wait_first_expose(t_xvar *xvar)
{
	XEvent	ev;

	XWindowEvent(xvar->display, xvar->window, ExposureMask, &ev);
	XPutBackEvent(xvar->display, &ev);
	return ;
}
