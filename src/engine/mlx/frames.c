#include "cimmerian.h"

static int	set_frame(t_man *man, t_frame *f, int is_swap_buf);

int	init_frames(t_man *man)
{
	int	i;

	if (!set_frame(man, &man->frame, 0))
		return (0);
	i = 0;
	while (i < 2)
	{
		if (!set_frame(man, man->swap_buf + i, 1))
			return (0);
		++i;
	}
	man->z_buf = malloc(man->frame.size.x * sizeof(double));
	if (!man->z_buf)
		return (put_error(man, E_FAIL_MEM, 0));
	return (1);
}

void	clear_frame(t_man *man)
{
	bzero(man->frame.addr, man->frame.size.x * man->frame.size.y
		* (man->frame.bpp / 8));
	return ;
}

void	free_frames(t_man *man)
{
	int	i;

	if (man->frame.img)
		mlx_destroy_image(man->mlx, man->frame.img);
	man->frame.img = 0;
	i = 0;
	while (i < 2)
	{
		if (man->swap_buf[i].img)
			mlx_destroy_image(man->mlx, man->swap_buf[i].img);
		man->swap_buf[i].img = 0;
		++i;
	}
	free(man->z_buf);
	man->z_buf = 0;
	return ;
}

static int	set_frame(t_man *man, t_frame *f, int is_swap_buf)
{
	if (is_swap_buf)
		set_ivec2(&f->size, man->res.viewport_size.x, man->res.viewport_size.y);
	else
		set_ivec2(&f->size, man->res.res.x, man->res.res.y);
	f->thickness = f->size.x / man->res.res.x;
	f->img = mlx_new_image(man->mlx, f->size.x, f->size.y);
	if (!f->img)
		return (put_error(man, E_FAIL_MLX_IMG, 0));
	f->addr = (t_ubyte *)mlx_get_data_addr(f->img, &f->bpp, &f->line_length,
			&f->endian);
	return (1);
}
