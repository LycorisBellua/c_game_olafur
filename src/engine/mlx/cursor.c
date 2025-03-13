#include "cimmerian.h"

static void	draw_cursor(t_spr *sprite, t_frame *frame, t_ivec2 p, int cyc);

int	on_mouse_move(t_win *win)
{
	t_ivec2	pos;
	t_frame	*f;

	mlx_mouse_get_pos(win->mlx, win->win, &pos.x, &pos.y);
	if (pos.x < 0 || pos.y < 0 || pos.x >= win->size.x || pos.y >= win->size.y)
	{
		set_ivec2(&win->cursor, -1, -1);
		return (0);
	}
	f = &win->frame[win->curr_frame];
	set_ivec2(&win->cursor, pos.x / f->thickness, pos.y / f->thickness);
	draw_cursor(&win->sprites[1], f, win->cursor, win->click);
	win->click = 0;
	return (0);
}

static void	draw_cursor(t_spr *sprite, t_frame *frame, t_ivec2 p, int cyc)
{
	size_t	i;
	size_t	len;
	t_ivec2	q;
	t_ivec2	r;
	t_ivec2	zoom_p;

	i = 0;
	len = sprite->size.x * sprite->size.y;
	while (i < len)
	{
		q.y = i / sprite->size.x;
		q.x = i - q.y * sprite->size.x;
		r.y = -1;
		while (++r.y < 2)
		{
			r.x = -1;
			while (++r.x < 2)
			{
				set_ivec2(&zoom_p, p.x + q.x * 2 + r.x, p.y + q.y * 2 + r.y);
				draw_point(frame, sprite->cycle[cyc][i], zoom_p);
			}
		}
		++i;
	}
	return ;
}
