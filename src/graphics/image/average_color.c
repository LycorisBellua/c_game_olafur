#include "cimmerian.h"

t_color	calculate_average_color(t_color *buf, size_t len)
{
	size_t	i;
	double	alpha;
	int		total_color[4];

	bzero(&total_color, 4 * sizeof(int));
	i = 0;
	while (i < len)
	{
		alpha = buf[i].a / 255.0;
		total_color[0] += buf[i].r * alpha;
		total_color[1] += buf[i].g * alpha;
		total_color[2] += buf[i].b * alpha;
		total_color[3] += buf[i].a;
		++i;
	}
	return (get_color_rgba(total_color[0] / len, total_color[1] / len,
			total_color[2] / len, total_color[3] / len));
}

int	calculate_sprite_average_color(t_spr *s)
{
	size_t	i;

	s->average_color = calloc(s->cycle_len, sizeof(t_color));
	s->is_see_through = calloc(s->cycle_len, sizeof(int));
	if (!s->average_color || !s->is_see_through)
		return (0);
	i = 0;
	while (i < s->cycle_len)
	{
		s->average_color[i] = calculate_average_color(s->cycle[i],
				s->size.x * s->size.y);
		s->is_see_through[i] = s->average_color[i].a < 255;
		++i;
	}
	return (1);
}
