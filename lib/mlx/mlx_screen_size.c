#include "mlx.h"

void	mlx_screen_size(t_xvar *xvar, int *x, int *y)
{
	XWindowAttributes	xw_attr;
	Status				ret;

	ret = XGetWindowAttributes(xvar->display, xvar->root, &xw_attr);
	*x = xw_attr.width;
	*y = xw_attr.height;
	return ;
}
