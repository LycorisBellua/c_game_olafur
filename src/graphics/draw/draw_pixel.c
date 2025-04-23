#include "cimmerian.h"

/*
	Checking for whether the new pixel is opaque is not necessary, the visual 
	result would have been the same with alpha blending. Skipping this part is 
	only done to improve performance.
*/
void	draw_pixel(t_color *buf, t_color c, t_ivec2 coord, t_ivec2 size)
{
	if (!buf || coord.x < 0 || coord.y < 0 || coord.x >= size.x
		|| coord.y >= size.y)
		return ;
	if (c.a < 255)
		c = alpha_blending(buf[coord.y * size.x + coord.x], c);
	buf[coord.y * size.x + coord.x] = c;
	return ;
}
