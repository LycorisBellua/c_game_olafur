#include "mlx_int.h"

int	mlx_window_fullscreen(t_xvar *xvar, t_win_list *win, int toggle)
{
	Atom	wm_state;
	Atom	fullscreen;
	XEvent	ev;

	wm_state = XInternAtom(xvar->display, "_NET_WM_STATE", False);
	fullscreen = XInternAtom(xvar->display, "_NET_WM_STATE_FULLSCREEN", False);
	bzero(&ev, sizeof(ev));
	ev.type = ClientMessage;
	ev.xclient.window = win->window;
	ev.xclient.message_type = wm_state;
	ev.xclient.format = 32;
	ev.xclient.data.l[0] = !!toggle;
	ev.xclient.data.l[1] = fullscreen;
	ev.xclient.data.l[2] = 0;
	ev.xclient.data.l[3] = 1;
	ev.xclient.data.l[4] = 0;
	XSendEvent(xvar->display, DefaultRootWindow(xvar->display), False,
		SubstructureRedirectMask | SubstructureNotifyMask, &ev);
	return (0);
}

