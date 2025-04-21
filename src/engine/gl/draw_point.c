#include "cimmerian.h"

void	draw_point(t_man *man, t_color c, int x, int y)
{
	if (x < 0 || y < 0 || x >= man->frame.size.x || y >= man->frame.size.y)
		return ;
	c = get_alpha_blended_color(man->frame.buf[y * man->frame.size.x + x], c);
	man->frame.buf[y * man->frame.size.x + x] = c;
	return ;
}
