#include "cimmerian.h"

static void	draw_shadow(t_frame *frame, t_img *img, t_ivec2 pos);

void	draw_image(t_frame *frame, t_img *img, t_ivec2 pos)
{
	size_t	i;
	size_t	len;
	t_ivec2	p;

	if (!img)
		return ;
	if (img->cycle_shadow)
		draw_shadow(frame, img, pos);
	i = 0;
	len = img->size.x * img->size.y;
	while (i < len)
	{
		p.y = i / img->size.x;
		p.x = i - p.y * img->size.x;
		set_ivec2(&p, p.x + pos.x, p.y + pos.y);
		draw_point(frame, img->cycle[img->cycle_index][i], p.x, p.y);
		++i;
	}
	return ;
}

void	fill_frame(t_frame *f, t_color c)
{
	t_ivec2	coord;

	coord.x = 0;
	while (coord.x < f->size.x)
	{
		coord.y = 0;
		while (coord.y < f->size.y)
		{
			draw_point(f, c, coord.x, coord.y);
			++coord.y;
		}
		++coord.x;
	}
	return ;
}

static void	draw_shadow(t_frame *frame, t_img *img, t_ivec2 pos)
{
	size_t	i;
	size_t	len;
	t_ivec2	p;

	pos.x += img->shadow_offset.x;
	pos.y += img->shadow_offset.y;
	i = 0;
	len = img->size.x * img->size.y;
	while (i < len)
	{
		p.y = i / img->size.x;
		p.x = i - p.y * img->size.x;
		set_ivec2(&p, p.x + pos.x, p.y + pos.y);
		draw_point(frame, img->cycle_shadow[img->cycle_index][i], p.x,
			p.y);
		++i;
	}
	return ;
}
