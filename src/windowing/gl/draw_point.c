#include "cimmerian.h"

/*
	Checking for whether the new pixel is opaque is not necessary, the visual 
	result would have been the same with alpha blending. Skipping this part is 
	only done to improve performance.
*/
void	draw_point(t_man *man, t_color c, int x, int y)
{
	if (x < 0 || y < 0 || x >= man->frame.size.x || y >= man->frame.size.y)
		return ;
	if (c.a < 255)
		c = alpha_blending(man->frame.buf[y * man->frame.size.x + x], c);
	man->frame.buf[y * man->frame.size.x + x] = c;
	return ;
}
