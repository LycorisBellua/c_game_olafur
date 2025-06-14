#include "sua.h"

void	sua_image_to_window(t_xvar *xvar, t_ximg *img, int x, int y)
{
	if (img->type == SUA_TYPE_SHM)
    	XShmPutImage(xvar->display, img->pix, xvar->gc, img->image, 0, 0, 0, 0,
			img->width, img->height, False);
	else if (img->type == SUA_TYPE_XIMAGE)
    	XPutImage(xvar->display, img->pix, xvar->gc, img->image, 0, 0, 0, 0,
			img->width, img->height);
	XCopyArea(xvar->display, img->pix, xvar->window, xvar->gc, 0, 0, img->width,
		img->height, x, y);
	return ;
}