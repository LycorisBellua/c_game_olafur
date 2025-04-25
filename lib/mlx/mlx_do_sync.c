#include "mlx.h"

int	mlx_do_sync(t_xvar *xvar)
{
	XSync(xvar->display, False);
	return (0);
}
