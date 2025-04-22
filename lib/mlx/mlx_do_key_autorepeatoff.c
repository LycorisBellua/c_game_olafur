#include	"mlx_int.h"

int	mlx_do_key_autorepeatoff(t_xvar *xvar)
{
  XAutoRepeatOff(xvar->display);
}
