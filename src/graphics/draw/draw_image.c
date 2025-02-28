#include "cimmerian.h"

void	draw_image(t_frame *f, t_img *img)
{
	t_color	c;
	t_ivec2	f_coord;

	f_coord.y = 0;
	while (f_coord.y < img->size.y)
	{
		f_coord.x = 0;
		while (f_coord.x < img->size.x)
		{
			c = img->buf[f_coord.y * img->size.x + f_coord.x];
			draw_point(f, c, f_coord.x, f_coord.y);
			++f_coord.x;
		}
		++f_coord.y;
	}
	return ;
}

void	draw_image_with_x_offset(t_frame *f, t_img *img, int x_offset)
{
	t_color	c;
	t_ivec2	f_coord;
	t_ivec2	i_coord;

	x_offset %= img->size.x;
	f_coord.y = 0;
	while (f_coord.y < img->size.y)
	{
		i_coord.y = f_coord.y;
		i_coord.x = x_offset;
		f_coord.x = 0;
		while (f_coord.x < img->size.x)
		{
			if (i_coord.x >= img->size.x)
				i_coord.x = 0;
			c = img->buf[i_coord.y * img->size.x + i_coord.x];
			draw_point(f, c, f_coord.x, f_coord.y);
			++i_coord.x;
			++f_coord.x;
		}
		++f_coord.y;
	}
	return ;
}

void	draw_background(t_frame *f, t_map *m)
{
	t_player	*p;
	double		angle;
	int			offset;

	p = &g_man.player;
	angle = get_angle_from_dir(p->dir.x, p->dir.y);
	offset = (angle + PI) / RAD_360 * m->background->size.x;
	draw_image_with_x_offset(f, m->background, offset);
	return ;
}
