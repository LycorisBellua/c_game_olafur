#include "cimmerian.h"

t_color	get_frame_pixel(t_frame *f, int x, int y)
{
	return (f->buf[y * f->real_size.x + x]);
}

void	set_frame_pixel(t_frame *f, t_color c, int x, int y)
{
	f->buf[y * f->real_size.x + x] = c;
	return ;
}
