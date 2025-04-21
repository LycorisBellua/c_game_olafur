#include "cimmerian.h"

t_color		get_frame_pixel(t_frame *f, int x, int y);
void		set_frame_pixel(t_frame *f, t_color c, int x, int y);

static void	upscale_frame(t_frame *f, t_frame *u);

void	display_frame(t_man *man)
{
	static int	curr_swap_buf;
	t_frame		*swap;

	swap = man->swap_buf + curr_swap_buf;
	upscale_frame(&man->frame, swap);
	mlx_put_image_to_window(man->mlx, man->window, swap->img,
		man->res.viewport_offset.x, man->res.viewport_offset.y);
	curr_swap_buf = (curr_swap_buf + 1) % 2;
	return ;
}

static void	upscale_frame(t_frame *f, t_frame *u)
{
	t_ivec2	coord;
	int		t;

	coord.y = -1;
	while (++coord.y < f->size.y)
	{
		coord.x = -1;
		while (++coord.x < f->size.x)
		{
			t = -1;
			while (++t < u->thickness)
				set_frame_pixel(u, get_frame_pixel(f, coord.x, coord.y),
					coord.x * u->thickness + t, coord.y * u->thickness);
		}
		t = 0;
		while (++t < u->thickness)
			memcpy(&u->addr[(coord.y * u->thickness + t) * u->line_length],
				&u->addr[coord.y * u->thickness * u->line_length],
				u->line_length);
	}
	return ;
}
