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

void	draw_png_with_x_offset(t_frame *f, t_png *png, int x_offset)
{
	t_color	c;
	t_ivec2	f_coord;
	t_ivec2	i_coord;

	x_offset %= png->size.x;
	if (x_offset < 0)
		x_offset += png->size.x;
	f_coord.y = 0;
	while (f_coord.y < png->size.y)
	{
		i_coord.y = f_coord.y;
		f_coord.x = 0;
		while (f_coord.x < f->size.x)
		{
			i_coord.x = (f_coord.x + x_offset) % png->size.x;
			c = png->buf[i_coord.y * png->size.x + i_coord.x];
			draw_point(f, c, f_coord.x, f_coord.y);
			++f_coord.x;
		}
		++f_coord.y;
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
