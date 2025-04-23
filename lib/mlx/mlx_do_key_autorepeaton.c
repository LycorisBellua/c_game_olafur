#include "mlx_int.h"

int	mlx_do_key_autorepeaton(t_xvar *xvar)
{
	XAutoRepeatOn(xvar->display);
	return (0);
}
