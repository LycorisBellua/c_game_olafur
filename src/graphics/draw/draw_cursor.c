#include "olafur.h"

void	draw_cursor(t_man *man, t_img *image, t_ivec2 p, int cyc)
{
	int		i;
	t_ivec2	q;
	t_ivec2	r;
	t_ivec2	zoom_p;

	if (!image)
		return ;
	i = -1;
	while (++i < image->size.x * image->size.y)
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
				draw_point(man, image->cycle[cyc][i], zoom_p.x, zoom_p.y);
			}
		}
	}
	return ;
}
