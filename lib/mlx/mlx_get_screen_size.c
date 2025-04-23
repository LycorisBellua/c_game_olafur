#include "mlx_int.h"

int	mlx_get_screen_size(void *mlx_ptr, int *sizex, int *sizey)
{
	XWindowAttributes	xw_attr;
	Status				ret;
	t_xvar				*xvar;

	xvar = mlx_ptr;
	ret = XGetWindowAttributes(xvar->display, xvar->root, &xw_attr);
	(*sizex) = xw_attr.width;
	(*sizey) = xw_attr.height;
	return (0);
}
