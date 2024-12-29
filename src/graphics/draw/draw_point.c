#include "cimmerian.h"

void	draw_point(t_frame *f, t_color c, int x, int y)
{
	int		row;
	int		col;

	if (x < 0 || y < 0 || x >= f->size.x || y >= f->size.y)
		return ;
	x *= f->thickness;
	y *= f->thickness;
	c = get_alpha_blended_color(f->buf[y * f->real_size.x + x], c);
	row = 0;
	while (row < f->thickness)
	{
		col = 0;
		while (col < f->thickness)
		{
			f->buf[(y + row) * f->real_size.x + (x + col)] = c;
			++col;
		}
		++row;
	}
	return ;
}

void	draw_point_img(t_img *img, t_color c, int x, int y)
{
	t_color	*buf;

	buf = img->buf + (y * img->size.x + x);
	*buf = get_alpha_blended_color(*buf, c);
	return ;
}
