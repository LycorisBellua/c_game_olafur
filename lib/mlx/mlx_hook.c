#include "mlx.h"

int	mlx_hook(t_win_list *win, int x_event, int x_mask, int (*funct)(),
		void *param)
{
	win->hooks[x_event].hook = funct;
	win->hooks[x_event].param = param;
	win->hooks[x_event].mask = x_mask;
	return (0);
}
