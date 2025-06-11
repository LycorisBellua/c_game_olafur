#include "cimmerian.h"

static void	apply_alpha(t_color *buf, t_ivec2 size, t_ubyte alpha);

t_png	*create_empty_png(t_ivec2 size, t_ubyte alpha)
{
	t_png	*png;

	png = calloc(1, sizeof(t_png));
	if (!png)
		return (0);
	set_ivec2(&png->size, size.x, size.y);
	png->buf = calloc(png->size.x * png->size.y, sizeof(t_color));
	if (!png->buf)
	{
		free_png(png);
		return (0);
	}
	apply_alpha(png->buf, size, alpha);
	return (png);
}

t_img	*create_empty_image(const char *id, t_ivec2 size, t_ubyte alpha)
{
	t_img	*img;

	if (!id)
		return (0);
	img = calloc(1, sizeof(t_img));
	if (!img)
		return (0);
	img->id = strdup(id);
	set_ivec2(&img->size, size.x, size.y);
	img->cycle_len = 1;
	img->cycle = calloc(img->cycle_len, sizeof(t_color *));
	if (img->cycle)
		img->cycle[0] = calloc(img->size.x * img->size.y, sizeof(t_color));
	img->average_color = calloc(img->cycle_len, sizeof(t_color));
	img->is_see_through = calloc(img->cycle_len, sizeof(int));
	if (!img->id || !img->cycle || !img->cycle[0] || !img->average_color
		|| !img->is_see_through)
	{
		free_image(img, free);
		return (0);
	}
	apply_alpha(img->cycle[0], size, alpha);
	img->average_color[0].a = alpha;
	img->is_see_through[0] = alpha < 255;
	return (img);
}

static void	apply_alpha(t_color *buf, t_ivec2 size, t_ubyte alpha)
{
	int	i;
	int	len;

	if (!buf || !alpha)
		return ;
	i = 0;
	len = size.x * size.y;
	while (i < len)
		buf[i++].a = alpha;
	return ;
}
