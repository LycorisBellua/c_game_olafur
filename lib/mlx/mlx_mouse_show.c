#include "mlx.h"

void	mlx_mouse_show(t_xvar *xvar)
{
	XUndefineCursor(xvar->display, xvar->window);
	return ;
}
