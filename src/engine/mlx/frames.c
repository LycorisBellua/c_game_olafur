#include "cimmerian.h"

int			set_swap_buf_frames(t_man *man);
int			set_xmap_and_ymap(t_man *man);
void		free_swap_buf_frames(t_man *man);

static int	set_frame(t_man *man, t_frame *f, int is_swap_buf);

int	init_frames(t_man *man)
{
	if (!set_frame(man, &man->frame, 0) || !set_swap_buf_frames(man)
		|| !set_xmap_and_ymap(man))
		return (put_error(man, 0, 0));
	man->z_buf = malloc(man->frame.size.x * sizeof(double));
	if (!man->z_buf)
		return (put_error(man, E_FAIL_MEM, 0));
	return (1);
}

int	set_swap_buf_frames(t_man *man)
{
	int	i;

	i = 0;
	while (i < 2)
	{
		if (!set_frame(man, man->swap_buf + i, 1))
			return (0);
		++i;
	}
	return (1);
}

void	free_frames(t_man *man)
{
	if (man->frame.img)
		mlx_image_destroy(man->mlx, man->frame.img);
	man->frame.img = 0;
	free(man->frame.xmap);
	free(man->frame.ymap);
	man->frame.xmap = 0;
	man->frame.ymap = 0;
	free_swap_buf_frames(man);
	free(man->z_buf);
	man->z_buf = 0;
	return ;
}

void	free_swap_buf_frames(t_man *man)
{
	int	i;

	i = 0;
	while (i < 2)
	{
		if (man->swap_buf[i].img)
			mlx_image_destroy(man->mlx, man->swap_buf[i].img);
		man->swap_buf[i].img = 0;
		++i;
	}
	return ;
}

static int	set_frame(t_man *man, t_frame *f, int is_swap_buf)
{
	if (is_swap_buf)
		set_ivec2(&f->size, man->res.viewport_size.x, man->res.viewport_size.y);
	else
		set_ivec2(&f->size, man->res.res.x, man->res.res.y);
	f->thickness = (double)f->size.x / man->res.res.x;
	f->img = mlx_image_create(man->mlx, f->size.x, f->size.y);
	if (!f->img)
		return (put_error(man, E_FAIL_MLX_IMG, 0));
	f->addr = (t_ubyte *)mlx_get_data_addr(f->img, &f->bpp, &f->line_length,
			&f->endian);
	return (1);
}
