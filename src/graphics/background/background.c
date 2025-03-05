#include "cimmerian.h"

static void	draw_png_with_x_offset(t_frame *f, t_png *png, int x_offset);

void	compose_background(t_map *m)
{
	m->background = create_empty_png(g_man.res.window_size_default);
	if (m->background)
		update_background(m);
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
	draw_png_with_x_offset(f, m->background, offset);
	return ;
}

static void	draw_png_with_x_offset(t_frame *f, t_png *png, int x_offset)
{
	t_color	c;
	t_ivec2	f_coord;
	t_ivec2	i_coord;

	x_offset %= png->size.x;
	f_coord.y = 0;
	while (f_coord.y < png->size.y)
	{
		i_coord.y = f_coord.y;
		i_coord.x = x_offset;
		f_coord.x = 0;
		while (f_coord.x < png->size.x)
		{
			if (i_coord.x >= png->size.x)
				i_coord.x = 0;
			c = png->buf[i_coord.y * png->size.x + i_coord.x];
			draw_point(f, c, f_coord.x, f_coord.y);
			++i_coord.x;
			++f_coord.x;
		}
		++f_coord.y;
	}
	return ;
}
