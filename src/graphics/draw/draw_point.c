#include "cimmerian.h"

void	draw_point(t_frame *f, t_color c, int x, int y)
{
	int	row;
	int	col;

	if (!f || x < 0 || y < 0 || x >= f->size.x || y >= f->size.y)
		return ;
	x *= f->thickness;
	y *= f->thickness;
	c = get_alpha_blended_color(get_frame_pixel(f, x, y), c);
	row = 0;
	while (row < f->thickness)
	{
		col = 0;
		while (col < f->thickness)
		{
			set_frame_pixel(f, c, x + col, y + row);
			++col;
		}
		++row;
	}
	return ;
}
