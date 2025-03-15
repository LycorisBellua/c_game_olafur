#include "cimmerian.h"

static int	set_frame(t_man *man, t_frame *f);

int	init_frames(t_man *man)
{
	size_t	i;

	man->curr_frame = 0;
	i = 0;
	while (i < NBR_FRAMES)
	{
		if (!set_frame(man, man->frame + i))
		{
			free_frames(man);
			return (0);
		}
		++i;
	}
	return (1);
}

void	clear_frame(t_frame *f)
{
	bzero(f->addr, f->real_size.x * f->real_size.y * (f->bpp / 8));
	return ;
}

void	display_frame(t_man *man, t_frame *f)
{
	mlx_put_image_to_window(man->mlx, man->window, f->img, 0, 0);
	return ;
}

void	free_frames(t_man *man)
{
	size_t	i;

	i = 0;
	while (i < NBR_FRAMES)
	{
		if (man->frame[i].img)
			mlx_destroy_image(man->mlx, man->frame[i].img);
		man->frame[i].img = 0;
		++i;
	}
	return ;
}

static int	set_frame(t_man *man, t_frame *f)
{
	set_ivec2(&f->size, man->res.window_size_default.x,
		man->res.window_size_default.y);
	set_ivec2(&f->real_size, man->res.window_size.x, man->res.window_size.y);
	f->thickness = f->real_size.x / f->size.x;
	f->img = mlx_new_image(man->mlx, f->real_size.x, f->real_size.y);
	if (!f->img)
		return (0);
	f->addr = (t_ubyte *)mlx_get_data_addr(f->img, &f->bpp, &f->line_length,
		&f->endian);
	return (1);
}
