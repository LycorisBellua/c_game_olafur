#include "cimmerian.h"

t_color	get_frame_pixel(t_frame *f, int x, int y)
{
	t_ubyte	*dst;

	dst = f->addr + (y * f->line_length + x * (f->bpp / 8));
	if (f->endian)
		return (get_color_rgba(dst[1], dst[2], dst[3], dst[0]));
	return (get_color_rgba(dst[2], dst[1], dst[0], dst[3]));
	//return (f->buf[y * f->real_size.x + x]);
}

void	set_frame_pixel(t_frame *f, t_color c, int x, int y)
{
	t_ubyte	*dst;

	dst = f->addr + (y * f->line_length + x * (f->bpp / 8));
	if (f->endian)
		c = get_color_rgba(c.a, c.r, c.g, c.b);
	else
		c = get_color_rgba(c.b, c.g, c.r, c.a);
	*(t_uint *)dst = c;
	//f->buf[y * f->real_size.x + x] = c;
	/*
	t_ubyte	color[4];
	if (f->endian)
	{
		color[0] = c.a;
		color[1] = c.r;
		color[2] = c.g;
		color[3] = c.b;
	}
	else
	{
		color[0] = c.b;
		color[1] = c.g;
		color[2] = c.r;
		color[3] = c.a;
	}
	*(t_uint *)dst = color;
	*/
	return ;
}
