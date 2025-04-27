#include "mlx.h"

void	mlx_mouse_hide(t_xvar *xvar)
{
	static char	data[1] = {0};
	Cursor		cursor;
	Pixmap		blank;
	XColor		dummy;

	blank = XCreateBitmapFromData(xvar->display, xvar->window, data, 1, 1);
	if (blank == None)
		return ;
	cursor = XCreatePixmapCursor(xvar->display, blank, blank, &dummy, &dummy,
			0, 0);
	if (cursor == None)
	{
		XFreePixmap(xvar->display, blank);
		return ;
	}
	XDefineCursor(xvar->display, xvar->window, cursor);
	XFreePixmap(xvar->display, blank);
	XFreeCursor(xvar->display, cursor);
	return ;
}
