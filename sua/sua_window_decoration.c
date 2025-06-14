#include "sua.h"

static void	force_reapply(t_xvar *xvar);

void	sua_window_decoration(t_xvar *xvar, int toggle)
{
	Atom			prop;
	MotifWmHints	hints;

	prop = XInternAtom(xvar->display, "_MOTIF_WM_HINTS", False);
	hints.flags = MWM_HINTS_DECORATIONS;
	hints.decorations = !!toggle;
	hints.functions = 0;
	hints.input_mode = 0;
	hints.status = 0;
	XChangeProperty(xvar->display, xvar->window, prop, prop, 32,
		PropModeReplace, (unsigned char *)&hints, 5);
	force_reapply(xvar);
	return ;
}

static void	force_reapply(t_xvar *xvar)
{
	XUnmapWindow(xvar->display, xvar->window);
	XMapRaised(xvar->display, xvar->window);
	XFlush(xvar->display);
	return ;
}
