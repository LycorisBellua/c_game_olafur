#include "olafur.h"

void	draw_point(t_man *man, t_color c, int x, int y)
{
	t_color	*buf;

	if (x < 0 || y < 0 || x >= man->frame.size.x || y >= man->frame.size.y)
		return ;
	buf = man->frame.buf + (y * man->frame.size.x + x);
	if (c.a < 255)
		c = alpha_blending(*buf, c);
	*buf = c;
	return ;
}

void	draw_scaled_point(t_man *man, t_color c, t_ivec2 at, int scale)
{
	int	bx;
	int	by;

	by = -1;
	while (++by < scale)
	{
		bx = -1;
		while (++bx < scale)
			draw_point(man, c, at.x + bx, at.y + by);
	}
	return ;
}

void	draw_point_fast(t_man *man, t_color c, int x, int y)
{
	t_color	*buf;

	buf = man->frame.buf + (y * man->frame.size.x + x);
	if (c.a < 255)
		c = alpha_blending(*buf, c);
	*buf = c;
	return ;
}
