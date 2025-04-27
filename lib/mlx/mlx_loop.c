#include "mlx.h"

extern	void (*(mlx_int_param_event[]))();

static void	mlx_int_set_win_event_mask(t_xvar *xvar);

void	mlx_loop(t_xvar *xvar)
{
	XEvent	ev;

	mlx_int_set_win_event_mask(xvar);
	while (xvar->window && !xvar->end_loop)
	{
		while (!xvar->end_loop && (!xvar->loop_hook || XPending(xvar->display)))
		{
			XNextEvent(xvar->display, &ev);
			if (xvar->window && xvar->window == ev.xany.window)
			{
				if (ev.type == ClientMessage
						&& ev.xclient.message_type == xvar->wm_protocols
						&& ev.xclient.data.l[0] == xvar->wm_delete_window)
				{
					xvar->end_loop = 1;
					if (xvar->hooks[DestroyNotify].hook)
						xvar->hooks[DestroyNotify]
							.hook(xvar->hooks[DestroyNotify].param);
				}
				if (xvar->window && ev.type < LASTEvent
					&& xvar->hooks[ev.type].hook)
					mlx_int_param_event[ev.type](xvar, &ev);
			}
		}
		XSync(xvar->display, False);
		if (xvar->loop_hook)
			xvar->loop_hook(xvar->loop_param);
	}
	return ;
}

static void	mlx_int_set_win_event_mask(t_xvar *xvar)
{
	int						i;
	XSetWindowAttributes	xwa;

	if (!xvar->window)
		return ;
	xwa.event_mask = 0;
	i = LASTEvent;
	while (i--)
		xwa.event_mask |= xvar->hooks[i].mask;
	XChangeWindowAttributes(xvar->display, xvar->window, CWEventMask, &xwa);
	return ;
}
