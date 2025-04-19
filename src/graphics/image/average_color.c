#include "cimmerian.h"

static t_color	calculate_average_color(t_color *buf, size_t len);

int	calculate_image_average_color(t_img *img)
{
	size_t	i;

	if (!img)
		return (0);
	if (!img->average_color)
		img->average_color = calloc(img->cycle_len, sizeof(t_color));
	if (!img->is_see_through)
		img->is_see_through = calloc(img->cycle_len, sizeof(int));
	if (!img->average_color || !img->is_see_through)
		return (0);
	i = 0;
	while (i < img->cycle_len)
	{
		img->average_color[i] = calculate_average_color(img->cycle[i],
				img->size.x * img->size.y);
		img->is_see_through[i] = img->average_color[i].a < 255;
		++i;
	}
	return (1);
}

static t_color	calculate_average_color(t_color *buf, size_t len)
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
