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

int	set_xmap_and_ymap(t_man *man)
{
	t_ivec2	coord;

	man->frame.xmap = malloc(man->swap_buf[0].size.x * sizeof(int));
	man->frame.ymap = malloc(man->swap_buf[0].size.y * sizeof(int));
	if (!man->frame.xmap || !man->frame.ymap)
		return (0);
	coord.x = -1;
	while (++coord.x < man->swap_buf[0].size.x)
		man->frame.xmap[coord.x] = \
			clamp((int)(coord.x / man->swap_buf[0].thickness + 0.5),
				0, man->frame.size.x - 1);
	coord.y = -1;
	while (++coord.y < man->swap_buf[0].size.y)
		man->frame.ymap[coord.y] = \
			clamp((int)(coord.y / man->swap_buf[0].thickness + 0.5),
				0, man->frame.size.y - 1);
	return (1);
}

static void	upscale_frame(t_frame *f, t_frame *u)
{
	t_ivec2	f_coord;
	t_ivec2	u_coord;

	u_coord.y = -1;
	while (++u_coord.y < u->size.y)
	{
		f_coord.y = f->ymap[u_coord.y];
		u_coord.x = -1;
		while (++u_coord.x < u->size.x)
		{
			f_coord.x = f->xmap[u_coord.x];
			set_frame_pixel(u, get_frame_pixel(f, f_coord.x, f_coord.y),
				u_coord.x, u_coord.y);
		}
	}
	return ;
}
