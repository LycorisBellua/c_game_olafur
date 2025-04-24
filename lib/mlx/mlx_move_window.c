#include "mlx_int.h"

int	mlx_move_window(t_xvar *xvar, t_win_list *win, int x, int y)
{
	XMoveWindow(xvar->display, win->window, x, y);
	return (0);
}
