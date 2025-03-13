#include "cimmerian.h"

static void	put_pixel(t_frame *f, t_color c, int x, int y);

void	draw_point(t_frame *f, t_color c, int x, int y)
{
	int	row;
	int	col;

	if (!f || x < 0 || y < 0 || x >= f->size.x || y >= f->size.y)
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
			put_pixel(f, c, x + col, y + row);
			++col;
		}
		++row;
	}
	return ;
}

/*
#ifdef MLX
static void	put_pixel(t_frame *f, t_color c, int x, int y)
{
	char	*dst;
	t_ubyte	current[4];
	t_ubyte	new[4];
	t_uint	color;

	//from `t_color c` to `t_uint color`

	dst = f->addr + (y * f->line_length + x * (f->bpp / 8));
	new[0] = get_alpha(color);
	new[1] = get_red(color);
	new[2] = get_green(color);
	new[3] = get_blue(color);
	if (f->endian)
		*(t_uint *)dst = get_alpha_blended_color((t_ubyte *)dst, new);
	else
	{
		current[0] = dst[3];
		current[1] = dst[2];
		current[2] = dst[1];
		current[3] = dst[0];
		*(t_uint *)dst = get_alpha_blended_color(current, new);
	}
	return ;
}
#else
*/
static void	put_pixel(t_frame *f, t_color c, int x, int y)
{
	f->buf[y * f->real_size.x + x] = c;
	return ;
}
//#endif
