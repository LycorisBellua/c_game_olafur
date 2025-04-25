#include "mlx.h"

void	mlx_do_sync(t_xvar *xvar)
{
	XSync(xvar->display, False);
	return ;
}
