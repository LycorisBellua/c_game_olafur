#include "olafur.h"

t_color	get_frame_pixel(t_frame *f, int x, int y);
void	set_frame_pixel(t_frame *f, t_color c, int x, int y);

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
		c = alpha_blending(get_frame_pixel(&man->frame, x, y), c);
	set_frame_pixel(&man->frame, c, x, y);
	return ;
}

t_color	get_frame_pixel(t_frame *f, int x, int y)
{
	char	*dst;

	dst = f->img->data + (y * f->img->size_line + x * (f->img->bpp / 8));
	if (f->img->image->byte_order)
		return (get_color_rgba(dst[1], dst[2], dst[3], dst[0]));
	return (get_color_rgba(dst[2], dst[1], dst[0], dst[3]));
}

void	set_frame_pixel(t_frame *f, t_color c, int x, int y)
{
	char	*dst;

	dst = f->img->data + (y * f->img->size_line + x * (f->img->bpp / 8));
	if (f->img->image->byte_order)
		c = get_color_rgba(c.a, c.r, c.g, c.b);
	else
		c = get_color_rgba(c.b, c.g, c.r, c.a);
	*(t_uint *)dst = *((t_uint *)&c);
	return ;
}
