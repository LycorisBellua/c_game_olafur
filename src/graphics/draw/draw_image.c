#include "olafur.h"

static void	draw_shadow(t_man *man, t_img *img, t_ivec2 pos, int scale);

void	draw_image(t_man *man, t_img *img, t_ivec2 pos)
{
	draw_image_scaled(man, img, pos, man->gui_scale);
	return ;
}

void	draw_image_scaled(t_man *man, t_img *img, t_ivec2 pos, int scale)
{
	size_t	i;
	size_t	len;
	t_ivec2	p;
	t_ivec2	at;

	if (!img)
		return ;
	if (img->cycle_shadow)
		draw_shadow(man, img, pos, scale);
	i = 0;
	len = img->size.x * img->size.y;
	while (i < len)
	{
		p.x = i / img->size.y;
		p.y = i - p.x * img->size.y;
		set_ivec2(&at, pos.x + p.x * scale, pos.y + p.y * scale);
		draw_scaled_point(man, img->cycle[img->cycle_index][i], at, scale);
		++i;
	}
	return ;
}

void	fill_frame(t_man *man, t_color c)
{
	int	i;
	int	len;

	i = 0;
	len = man->frame.size.x * man->frame.size.y;
	while (i < len)
		man->frame.buf[i++] = c;
	return ;
}

static void	draw_shadow(t_man *man, t_img *img, t_ivec2 pos, int scale)
{
	t_img	shadow;

	shadow = *img;
	shadow.cycle = img->cycle_shadow;
	shadow.cycle_shadow = 0;
	set_ivec2(&pos, pos.x + img->shadow_offset.x * scale,
		pos.y + img->shadow_offset.y * scale);
	draw_image_scaled(man, &shadow, pos, scale);
	return ;
}
