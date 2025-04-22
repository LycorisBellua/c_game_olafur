#include	"mlx_int.h"

extern int	(*(mlx_int_param_event[]))();

static int	win_count(t_xvar *xvar);
static int	mlx_int_set_win_event_mask(t_xvar *xvar);

int			mlx_loop(t_xvar *xvar)
{
	XEvent		ev;
	t_win_list	*win;

	mlx_int_set_win_event_mask(xvar);
	xvar->do_flush = 0;
	while (win_count(xvar) && !xvar->end_loop)
	{
		while (!xvar->end_loop && (!xvar->loop_hook || XPending(xvar->display)))
		{
			XNextEvent(xvar->display,&ev);
			win = xvar->win_list;
			while (win && (win->window!=ev.xany.window))
				win = win->next;

			if (win && ev.type == ClientMessage
				&& ev.xclient.message_type == xvar->wm_protocols
				&& ev.xclient.data.l[0] == xvar->wm_delete_window
				&& win->hooks[DestroyNotify].hook)
				win->hooks[DestroyNotify].hook(win->hooks[DestroyNotify].param);
			if (win && ev.type < MLX_MAX_EVENT && win->hooks[ev.type].hook)
				mlx_int_param_event[ev.type](xvar, &ev, win);
		}
		XSync(xvar->display, False);
		if (xvar->loop_hook)
			xvar->loop_hook(xvar->loop_param);
	}
	return (0);
}

static int	win_count(t_xvar *xvar)
{
	int			i;
	t_win_list	*win;

	i = 0;
	win = xvar->win_list;
	while (win)
	{
		win = win->next;
		++i;
	}
	return (i);
}

static int	mlx_int_set_win_event_mask(t_xvar *xvar)
{
  t_win_list	*win;
  int		mask;
  int		i;
  XSetWindowAttributes	xwa;

  win = xvar->win_list;
  while (win)
    {
      xwa.event_mask = 0;
      i = MLX_MAX_EVENT;
      while (i--)
	xwa.event_mask |= win->hooks[i].mask;
      XChangeWindowAttributes(xvar->display, win->window, CWEventMask, &xwa);
      win = win->next;
    }
	return (0);
}
