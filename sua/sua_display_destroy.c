#include "sua.h"

void	sua_display_destroy(t_xvar *xvar)
{
	XCloseDisplay(xvar->display);
	return ;
}
