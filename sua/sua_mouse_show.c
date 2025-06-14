#include "sua.h"

void	sua_mouse_show(t_xvar *xvar)
{
	XUndefineCursor(xvar->display, xvar->window);
	return ;
}
