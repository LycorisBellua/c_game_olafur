#include "cimmerian.h"

int	cross_window_button_callback(t_man *man)
{
	mlx_loop_end(man->mlx);
	return (0);
}
