#include "mlx.h"

int	mlx_loop_end(t_xvar *xvar)
{
	xvar->end_loop = 1;
	return (1);
}
