#include "mlx.h"

void	mlx_window_clear(t_xvar *xvar)
{
	XClearWindow(xvar->display, xvar->window);
	return ;
}
