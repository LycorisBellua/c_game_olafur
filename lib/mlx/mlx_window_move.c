#include "mlx.h"

int	mlx_window_move(t_xvar *xvar, t_win_list *win, int x, int y)
{
	XMoveWindow(xvar->display, win->window, x, y);
	return (0);
}
