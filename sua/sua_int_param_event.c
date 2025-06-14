#include "sua.h"

static void	sua_undef(void);
static void	sua_KeyPress(t_xvar *xvar, XEvent *ev);
static void	sua_KeyRelease(t_xvar *xvar, XEvent *ev);
static void	sua_ButtonPress(t_xvar *xvar, XEvent *ev);
static void	sua_ButtonRelease(t_xvar *xvar, XEvent *ev);
static void	sua_MotionNotify(t_xvar *xvar, XEvent *ev);
static void	sua_Expose(t_xvar *xvar, XEvent *ev);
static void	sua_ConfigureNotify(t_xvar *xvar, XEvent *ev);
static void	sua_ResizeRequest(t_xvar *xvar, XEvent *ev);
static void	sua_generic(t_xvar *xvar, XEvent *ev);

/* Range is [0 - 35] */
void	(*(sua_int_param_event[]))() =
{
	sua_undef,
	sua_undef,
	sua_KeyPress,
	sua_KeyRelease,
	sua_ButtonPress,
	sua_ButtonRelease,
	sua_MotionNotify,
	sua_generic,
	sua_generic,
	sua_generic,
	sua_generic,
	sua_generic,
	sua_Expose,
	sua_generic,
	sua_generic,
	sua_generic,
	sua_generic,
	sua_generic,
	sua_generic,
	sua_generic,
	sua_generic,
	sua_generic,
	sua_ConfigureNotify,
	sua_generic,
	sua_generic,
	sua_ResizeRequest,
	sua_generic,
	sua_generic,
	sua_generic,
	sua_generic,
	sua_generic,
	sua_generic,
	sua_generic,
	sua_generic,
	sua_generic,
	sua_generic
};

static void	sua_undef(void)
{
	return ;
}

static void	sua_KeyPress(t_xvar *xvar, XEvent *ev)
{
	xvar->hooks[KeyPress].hook(XkbKeycodeToKeysym(xvar->display,
		ev->xkey.keycode, 0, 0), xvar->hooks[KeyPress].param);
	return ;
}

static void	sua_KeyRelease(t_xvar *xvar, XEvent *ev)
{
	xvar->hooks[KeyRelease].hook(XkbKeycodeToKeysym(xvar->display,
		ev->xkey.keycode, 0, 0), xvar->hooks[KeyRelease].param);
	return ;
}

static void	sua_ButtonPress(t_xvar *xvar, XEvent *ev)
{
	xvar->hooks[ButtonPress].hook(ev->xbutton.button, ev->xbutton.x,
		ev->xbutton.y, xvar->hooks[ButtonPress].param);
	return ;
}

static void	sua_ButtonRelease(t_xvar *xvar, XEvent *ev)
{
	xvar->hooks[ButtonRelease].hook(ev->xbutton.button, ev->xbutton.x,
		ev->xbutton.y, xvar->hooks[ButtonRelease].param);
	return ;
}

static void	sua_MotionNotify(t_xvar *xvar, XEvent *ev)
{
	xvar->hooks[MotionNotify].hook(ev->xbutton.x, ev->xbutton.y,
		xvar->hooks[MotionNotify].param);
	return ;
}

static void	sua_Expose(t_xvar *xvar, XEvent *ev)
{
	if (!ev->xexpose.count)
		xvar->hooks[Expose].hook(xvar->hooks[Expose].param);
	return ;
}

static void	sua_ConfigureNotify(t_xvar *xvar, XEvent *ev)
{
	xvar->hooks[ConfigureNotify].hook(ev->xconfigure.x, ev->xconfigure.y,
		ev->xconfigure.width, ev->xconfigure.height,
		xvar->hooks[ConfigureNotify].param);
	return ;
}

static void	sua_ResizeRequest(t_xvar *xvar, XEvent *ev)
{
	if (xvar->hooks[ResizeRequest].hook)
		xvar->hooks[ResizeRequest].hook(ev->xresizerequest.width,
			ev->xresizerequest.height, xvar->hooks[ResizeRequest].param);
	return ;
}

static void	sua_generic(t_xvar *xvar, XEvent *ev)
{
	xvar->hooks[ev->type].hook(xvar->hooks[ev->type].param);
	return ;
}
