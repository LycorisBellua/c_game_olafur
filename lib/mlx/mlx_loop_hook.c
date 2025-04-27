#include "mlx.h"

void	mlx_loop_hook(t_xvar *xvar, void (*funct)(), void *param)
{
	xvar->loop_hook = funct;
	xvar->loop_param = param;
	return ;
}
