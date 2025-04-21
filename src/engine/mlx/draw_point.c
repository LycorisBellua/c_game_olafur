#include "cimmerian.h"

t_color	get_frame_pixel(t_frame *f, int x, int y);
void	set_frame_pixel(t_frame *f, t_color c, int x, int y);

void	draw_point(t_man *man, t_color c, int x, int y)
{
	if (x < 0 || y < 0 || x >= man->frame.size.x || y >= man->frame.size.y)
		return ;
	c = get_alpha_blended_color(get_frame_pixel(&man->frame, x, y), c);
	set_frame_pixel(&man->frame, c, x, y);
	return ;
}

t_color	get_frame_pixel(t_frame *f, int x, int y)
{
	t_ubyte	*dst;

	dst = f->addr + (y * f->line_length + x * (f->bpp / 8));
	if (f->endian)
		return (get_color_rgba(dst[1], dst[2], dst[3], dst[0]));
	return (get_color_rgba(dst[2], dst[1], dst[0], dst[3]));
}

void	set_frame_pixel(t_frame *f, t_color c, int x, int y)
{
	t_ubyte	*dst;

	dst = f->addr + (y * f->line_length + x * (f->bpp / 8));
	if (f->endian)
		c = get_color_rgba(c.a, c.r, c.g, c.b);
	else
		c = get_color_rgba(c.b, c.g, c.r, c.a);
	*(t_uint *)dst = *((t_uint *)&c);
	return ;
}
