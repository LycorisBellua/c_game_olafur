#include "cimmerian.h"

void	draw_pixel(t_color *buf, t_color c, t_ivec2 coord, t_ivec2 size)
{
	t_color	*pixel;

	if (!buf || coord.x < 0 || coord.y < 0 || coord.x >= size.x
		|| coord.y >= size.y)
		return ;
	pixel = buf + (coord.y * size.x + coord.x);
	*pixel = get_alpha_blended_color(*pixel, c);
	return ;
}
