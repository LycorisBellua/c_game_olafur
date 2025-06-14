#include "sua.h"

void	sua_window_clear(t_xvar *xvar)
{
	XClearWindow(xvar->display, xvar->window);
	return ;
}
