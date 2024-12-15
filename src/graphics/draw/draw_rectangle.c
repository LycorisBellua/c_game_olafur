#include "cimmerian.h"

void	draw_rectangle(t_frame *f, t_vert v, t_ivec2 size)
{
	t_vert	v2;
	t_vert	v3;
	t_vert	v4;

	v2.color = v.color;
	v3.color = v.color;
	v4.color = v.color;
	v2.coord.x = v.coord.x + size.x - 1;
	v2.coord.y = v.coord.y;
	v3.coord.x = v.coord.x;
	v3.coord.y = v.coord.y + size.y - 1;
	v4.coord.x = v.coord.x + size.x - 1;
	v4.coord.y = v.coord.y + size.y - 1;
	draw_line(f, v, v2);
	++v2.coord.y;
	draw_line(f, v2, v4);
	--v4.coord.x;
	draw_line(f, v3, v4);
	++v.coord.y;
	--v3.coord.y;
	draw_line(f, v, v3);
	return ;
}

void	draw_rectangle_full(t_frame *f, t_vert v, t_ivec2 size)
{
	int		target_y;
	t_vert	v2;

	target_y = v.coord.y + size.y - 1;
	v2.coord.x = v.coord.x + size.x - 1;
	v2.coord.y = v.coord.y;
	v2.color = v.color;
	while (v.coord.y <= target_y)
	{
		draw_line(f, v, v2);
		++v.coord.y;
		++v2.coord.y;
	}
	return ;
}
