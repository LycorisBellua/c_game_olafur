#include "mlx.h"

static int	mlx_undef(void);
static int	mlx_KeyPress(t_xvar *xvar, XEvent *ev, t_win_list *win);
static int	mlx_KeyRelease(t_xvar *xvar, XEvent *ev, t_win_list *win);
static int	mlx_ButtonPress(t_xvar *xvar, XEvent *ev, t_win_list *win);
static int	mlx_ButtonRelease(t_xvar *xvar, XEvent *ev, t_win_list *win);
static int	mlx_MotionNotify(t_xvar *xvar, XEvent *ev, t_win_list *win);
static int	mlx_Expose(t_xvar *xvar, XEvent *ev, t_win_list *win);
static int	mlx_ConfigureNotify(t_xvar *xvar, XEvent *ev, t_win_list *win);
static int	mlx_ResizeRequest(t_xvar *xvar, XEvent *ev, t_win_list *win);
static int	mlx_generic(t_xvar *xvar, XEvent *ev, t_win_list *win);

/* Range is [0 - 35] */
int	(*(mlx_int_param_event[]))() =
{
	mlx_undef,
	mlx_undef,
	mlx_KeyPress,
	mlx_KeyRelease,
	mlx_ButtonPress,
	mlx_ButtonRelease,
	mlx_MotionNotify,
	mlx_generic,
	mlx_generic,
	mlx_generic,
	mlx_generic,
	mlx_generic,
	mlx_Expose,
	mlx_generic,
	mlx_generic,
	mlx_generic,
	mlx_generic,
	mlx_generic,
	mlx_generic,
	mlx_generic,
	mlx_generic,
	mlx_generic,
	mlx_ConfigureNotify,
	mlx_generic,
	mlx_generic,
	mlx_ResizeRequest,
	mlx_generic,
	mlx_generic,
	mlx_generic,
	mlx_generic,
	mlx_generic,
	mlx_generic,
	mlx_generic,
	mlx_generic,
	mlx_generic,
	mlx_generic
};

static int	mlx_undef(void)
{
	return (0);
}

static int	mlx_KeyPress(t_xvar *xvar, XEvent *ev, t_win_list *win)
{
	win->hooks[KeyPress].hook(XkbKeycodeToKeysym(xvar->display,
			ev->xkey.keycode, 0, 0), win->hooks[KeyPress].param);
	return (0);
}

static int	mlx_KeyRelease(t_xvar *xvar, XEvent *ev, t_win_list *win)
{
	win->hooks[KeyRelease].hook(XkbKeycodeToKeysym(xvar->display,
			ev->xkey.keycode, 0, 0), win->hooks[KeyRelease].param);
	return (0);
}

static int	mlx_ButtonPress(t_xvar *xvar, XEvent *ev, t_win_list *win)
{
	win->hooks[ButtonPress].hook(ev->xbutton.button, ev->xbutton.x,
		ev->xbutton.y, win->hooks[ButtonPress].param);
	return (0);
}

static int	mlx_ButtonRelease(t_xvar *xvar, XEvent *ev, t_win_list *win)
{
	win->hooks[ButtonRelease].hook(ev->xbutton.button, ev->xbutton.x,
		ev->xbutton.y, win->hooks[ButtonRelease].param);
	return (0);
}

static int	mlx_MotionNotify(t_xvar *xvar, XEvent *ev, t_win_list *win)
{
	win->hooks[MotionNotify].hook(ev->xbutton.x, ev->xbutton.y,
		win->hooks[MotionNotify].param);
	return (0);
}

static int	mlx_Expose(t_xvar *xvar, XEvent *ev, t_win_list *win)
{
	if (!ev->xexpose.count)
		win->hooks[Expose].hook(win->hooks[Expose].param);
	return (0);
}

static int	mlx_ConfigureNotify(t_xvar *xvar, XEvent *ev, t_win_list *win)
{
	win->hooks[ConfigureNotify].hook(ev->xconfigure.x, ev->xconfigure.y,
		ev->xconfigure.width, ev->xconfigure.height,
		win->hooks[ConfigureNotify].param);
	return (0);
}

static int	mlx_ResizeRequest(t_xvar *xvar, XEvent *ev, t_win_list *win)
{
	if (win->hooks[ResizeRequest].hook)
		win->hooks[ResizeRequest].hook(ev->xresizerequest.width,
			ev->xresizerequest.height, win->hooks[ResizeRequest].param);
	return (0);
}

static int	mlx_generic(t_xvar *xvar, XEvent *ev, t_win_list *win)
{
	win->hooks[ev->type].hook(win->hooks[ev->type].param);
	return (0);
}
