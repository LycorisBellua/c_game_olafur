#include "mlx_int.h"

static void	force_reapply(t_xvar *xvar, t_win_list *win);

int	mlx_window_decoration(t_xvar *xvar, t_win_list *win, int toggle)
{
	Atom			prop;
	MotifWmHints	hints;

	prop = XInternAtom(xvar->display, "_MOTIF_WM_HINTS", False);
	hints.flags = MWM_HINTS_DECORATIONS;
	hints.decorations = !!toggle;
	hints.functions = 0;
	hints.input_mode = 0;
	hints.status = 0;
	XChangeProperty(xvar->display, win->window, prop, prop, 32,
		PropModeReplace, (unsigned char *)&hints, 5);
	force_reapply(xvar, win);
	return (0);
}

static void	force_reapply(t_xvar *xvar, t_win_list *win)
{
	XUnmapWindow(xvar->display, win->window);
	XMapRaised(xvar->display, win->window);
	XFlush(xvar->display);
	return ;
}
