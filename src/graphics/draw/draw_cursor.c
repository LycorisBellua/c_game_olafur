#include "olafur.h"

void	draw_cursor(t_man *man, t_img *image, t_ivec2 p, int cyc)
{
	int		i;
	t_ivec2	q;
	int		s;
	t_color *cycle;

	if (!image)
		return ;
	cycle = image->cycle[cyc % image->cycle_len];
	s = 2 * man->gui_scale;
	i = -1;
	while (++i < image->size.x * image->size.y)
	{
		q.x = i / image->size.y;
		q.y = i - q.x * image->size.y;
		draw_scaled_point(man, cycle[i],
			(t_ivec2){p.x + q.x * s, p.y + q.y * s}, s);
	}
	return ;
}
