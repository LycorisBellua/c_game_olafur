#include "cimmerian.h"

static void	draw_cursor(t_spr *sprite, t_frame *frame, t_ivec2 p, int cyc);

int	on_mouse_move(t_man *man)
{
	t_ivec2	pos;
	t_frame	*f;

	mlx_mouse_get_pos(man->mlx, man->window, &pos.x, &pos.y);
	if (pos.x < 0 || pos.y < 0 || pos.x >= man->res.window_size.x
		|| pos.y >= man->res.window_size.y)
	{
		set_ivec2(&man->cursor, -1, -1);
		return (0);
	}
	f = &man->frame[man->curr_frame];
	set_ivec2(&man->cursor, pos.x / f->thickness, pos.y / f->thickness);
	draw_cursor(&man->sprites[1], f, man->cursor, man->click);
	man->click = 0;
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
