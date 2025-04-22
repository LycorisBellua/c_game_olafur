#include	"mlx_int.h"

int	mlx_do_sync(t_xvar *xvar)
{
  XSync(xvar->display, False);
}
