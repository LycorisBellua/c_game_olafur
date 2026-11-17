#include "olafur.h"

static void	draw_sprite_column(t_man *man, t_spr *s, t_color *frame, int x);
static void	blit_sprite_pixel(t_man *man, t_spr *s, t_color c, t_ivec2 screen);

void	render_sprite_column(t_man *man, t_spr *s, int x)
{
	int	tex_x;

	tex_x = (int)(256 * (x - (-s->size.x / 2 + s->screen_x)) * s->img->size.x
		/ s->size.x) / 256;
	if (tex_x < 0 || tex_x >= s->img->size.x)
		return ;
	draw_sprite_column(man, s,
		s->img->cycle[s->img->cycle_index] + tex_x * s->img->size.y, x);
	return ;
}

static void	draw_sprite_column(t_man *man, t_spr *s, t_color *frame, int x)
{
	long	rem;
	long	den;
	int		tex_y;
	t_ivec2	screen;

	den = (long)s->size.y * 256;
	set_ivec2(&screen, x, s->draw_start.y);
	rem = ((long)(screen.y - s->v_move_screen) * 256
		- (long)man->frame.size.y * 128
		+ (long)s->size.y * 128) * s->img->size.y;
	tex_y = rem / den;
	rem -= tex_y * den;
	while (screen.y < s->draw_end.y)
	{
		if (tex_y < 0 || tex_y >= s->img->size.y)
			return ;
		blit_sprite_pixel(man, s, frame[tex_y], screen);
		rem += (long)s->img->size.y * 256;
		while (rem >= den)
		{
			++tex_y;
			rem -= den;
		}
		++screen.y;
	}
	return ;
}

static void	blit_sprite_pixel(t_man *man, t_spr *s, t_color c, t_ivec2 screen)
{
	if (c.a == 0)
		return ;
	apply_fog_factor(&c, man->maps[man->curr_map]->fog_color, s->fog_factor8);
	draw_point_fast(man, c, screen.x, screen.y);
	return ;
}
