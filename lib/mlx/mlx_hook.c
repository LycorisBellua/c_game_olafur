#include "mlx.h"

void	mlx_hook(t_xvar *xvar, int x_event, int x_mask, int (*funct)(),
			void *param)
{
	xvar->hooks[x_event].hook = funct;
	xvar->hooks[x_event].param = param;
	xvar->hooks[x_event].mask = x_mask;
	return ;
}
