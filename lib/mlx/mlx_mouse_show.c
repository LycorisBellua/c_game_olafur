#include "mlx_int.h"

int		mlx_mouse_show(t_xvar *xvar, t_win_list *win)
{
	XUndefineCursor(xvar->display, win->window);
}
