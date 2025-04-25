#include "mlx.h"

static void	mlx_undef(void);
static void	mlx_KeyPress(t_xvar *xvar, XEvent *ev);
static void	mlx_KeyRelease(t_xvar *xvar, XEvent *ev);
static void	mlx_ButtonPress(t_xvar *xvar, XEvent *ev);
static void	mlx_ButtonRelease(t_xvar *xvar, XEvent *ev);
static void	mlx_MotionNotify(t_xvar *xvar, XEvent *ev);
static void	mlx_Expose(t_xvar *xvar, XEvent *ev);
static void	mlx_ConfigureNotify(t_xvar *xvar, XEvent *ev);
static void	mlx_ResizeRequest(t_xvar *xvar, XEvent *ev);
static void	mlx_generic(t_xvar *xvar, XEvent *ev);

/* Range is [0 - 35] */
void	(*(mlx_int_param_event[]))() =
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

static void	mlx_undef(void)
{
	return ;
}

static void	mlx_KeyPress(t_xvar *xvar, XEvent *ev)
{
	xvar->hooks[KeyPress].hook(XkbKeycodeToKeysym(xvar->display,
		ev->xkey.keycode, 0, 0), xvar->hooks[KeyPress].param);
	return ;
}

static void	mlx_KeyRelease(t_xvar *xvar, XEvent *ev)
{
	xvar->hooks[KeyRelease].hook(XkbKeycodeToKeysym(xvar->display,
		ev->xkey.keycode, 0, 0), xvar->hooks[KeyRelease].param);
	return ;
}

static void	mlx_ButtonPress(t_xvar *xvar, XEvent *ev)
{
	xvar->hooks[ButtonPress].hook(ev->xbutton.button, ev->xbutton.x,
		ev->xbutton.y, xvar->hooks[ButtonPress].param);
	return ;
}

static void	mlx_ButtonRelease(t_xvar *xvar, XEvent *ev)
{
	xvar->hooks[ButtonRelease].hook(ev->xbutton.button, ev->xbutton.x,
		ev->xbutton.y, xvar->hooks[ButtonRelease].param);
	return ;
}

static void	mlx_MotionNotify(t_xvar *xvar, XEvent *ev)
{
	xvar->hooks[MotionNotify].hook(ev->xbutton.x, ev->xbutton.y,
		xvar->hooks[MotionNotify].param);
	return ;
}

static void	mlx_Expose(t_xvar *xvar, XEvent *ev)
{
	if (!ev->xexpose.count)
		xvar->hooks[Expose].hook(xvar->hooks[Expose].param);
	return ;
}

static void	mlx_ConfigureNotify(t_xvar *xvar, XEvent *ev)
{
	xvar->hooks[ConfigureNotify].hook(ev->xconfigure.x, ev->xconfigure.y,
		ev->xconfigure.width, ev->xconfigure.height,
		xvar->hooks[ConfigureNotify].param);
	return ;
}

static void	mlx_ResizeRequest(t_xvar *xvar, XEvent *ev)
{
	if (xvar->hooks[ResizeRequest].hook)
		xvar->hooks[ResizeRequest].hook(ev->xresizerequest.width,
			ev->xresizerequest.height, xvar->hooks[ResizeRequest].param);
	return ;
}

static void	mlx_generic(t_xvar *xvar, XEvent *ev)
{
	xvar->hooks[ev->type].hook(xvar->hooks[ev->type].param);
	return ;
}
