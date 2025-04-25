#include "mlx.h"

int	mlx_display_destroy(t_xvar *xvar)
{
	XCloseDisplay(xvar->display);
	return (0);
}
