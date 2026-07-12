#include "olafur.h"

static void	draw_shadow(t_man *man, t_img *img, t_ivec2 pos);

void	draw_image(t_man *man, t_img *img, t_ivec2 pos)
{
	size_t	i;
	size_t	len;
	t_ivec2	p;
	int		s;

	if (!img)
		return ;
	s = man->gui_scale;
	if (img->cycle_shadow)
		draw_shadow(man, img, pos);
	i = 0;
	len = img->size.x * img->size.y;
	while (i < len)
	{
		p.x = i / img->size.y;
		p.y = i - p.x * img->size.y;
		draw_scaled_point(man, img->cycle[img->cycle_index][i],
			(t_ivec2){pos.x + p.x * s, pos.y + p.y * s}, s);
		++i;
	}
	return ;
}

void	draw_png_bg_with_x_offset(t_man *man, t_png *png, int x_offset)
{
	t_color	*row_ptr;
	t_ivec2	f_coord;
	int		ix;

	x_offset %= png->size.x;
	if (x_offset < 0)
		x_offset += png->size.x;
	f_coord.y = 0;
	while (f_coord.y < png->size.y && f_coord.y < man->frame.size.y)
	{
		row_ptr = png->buf + f_coord.y * png->size.x;
		ix = x_offset;
		f_coord.x = 0;
		while (f_coord.x < man->frame.size.x)
		{
			draw_point_fast(man, row_ptr[ix], f_coord.x, f_coord.y);
			++f_coord.x;
			if (++ix >= png->size.x)
				ix = 0;
		}
		++f_coord.y;
	}
	return ;
}

void	fill_frame(t_man *man, t_color c)
{
	t_ivec2	coord;

	coord.x = 0;
	while (coord.x < man->frame.size.x)
	{
		coord.y = 0;
		while (coord.y < man->frame.size.y)
		{
			draw_point(man, c, coord.x, coord.y);
			++coord.y;
		}
		++coord.x;
	}
	return ;
}

static void	draw_shadow(t_man *man, t_img *img, t_ivec2 pos)
{
	size_t	i;
	size_t	len;
	t_ivec2	p;
	int		s;

	s = man->gui_scale;
	pos.x += img->shadow_offset.x * s;
	pos.y += img->shadow_offset.y * s;
	i = 0;
	len = img->size.x * img->size.y;
	while (i < len)
	{
		p.x = i / img->size.y;
		p.y = i - p.x * img->size.y;
		draw_scaled_point(man, img->cycle_shadow[img->cycle_index][i],
			(t_ivec2){pos.x + p.x * s, pos.y + p.y * s}, s);
		++i;
	}
	return ;
}
