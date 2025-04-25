#include "mlx.h"

int	mlx_image_to_window(t_xvar *xvar, t_win_list *win, t_ximg *img, int x,
		int y)
{
	GC	gc;

	gc = win->gc;
	if (img->gc)
	{
		gc = img->gc;
		XSetClipOrigin(xvar->display, gc, x, y);
	}
	if (img->type == MLX_TYPE_SHM)
		XShmPutImage(xvar->display, img->pix, win->gc, img->image, 0, 0, 0, 0,
			img->width, img->height, False);
	if (img->type == MLX_TYPE_XIMAGE)
		XPutImage(xvar->display, img->pix, win->gc, img->image, 0, 0, 0, 0,
			img->width, img->height);
	XCopyArea(xvar->display, img->pix, win->window, gc, 0, 0, img->width,
		img->height, x, y);
	if (xvar->do_flush)
		XFlush(xvar->display);
	return (0);
}
