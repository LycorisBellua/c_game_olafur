#include "mlx.h"

void	mlx_loop_end(t_xvar *xvar)
{
	xvar->end_loop = 1;
	return ;
}
