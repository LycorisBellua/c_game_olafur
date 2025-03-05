#include "cimmerian.h"

void	draw_cursor(t_frame *frame, t_img *image, t_ivec2 p, int cyc)
{
	size_t	i;
	size_t	len;
	t_ivec2	q;
	t_ivec2	r;
	t_ivec2	zoom_p;

	i = 0;
	len = image->size.x * image->size.y;
	while (i < len)
	{
		q.y = i / image->size.x;
		q.x = i - q.y * image->size.x;
		r.y = -1;
		while (++r.y < 2)
		{
			r.x = -1;
			while (++r.x < 2)
			{
				set_ivec2(&zoom_p, p.x + q.x * 2 + r.x, p.y + q.y * 2 + r.y);
				draw_point(frame, image->cycle[cyc][i], zoom_p.x, zoom_p.y);
			}
		}
		++i;
	}
	return ;
}
