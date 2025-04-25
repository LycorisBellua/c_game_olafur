#include "cimmerian.h"

static int	set_frame(t_man *man, t_frame *f, int is_swap_buf);
static int	set_xmap_and_ymap(t_man *man);

int	init_frame(t_man *man)
{
	int	i;

	if (!set_frame(man, &man->frame, 0))
		return (put_error(man, 0, 0));
	i = 0;
	while (i < 2)
	{
		if (!set_frame(man, man->swap_buf + i, 1))
			return (put_error(man, 0, 0));
		++i;
	}
	if (!set_xmap_and_ymap(man))
		return (put_error(man, 0, 0));
	man->z_buf = malloc(man->frame.size.x * sizeof(double));
	if (!man->z_buf)
		return (put_error(man, E_FAIL_MEM, 0));
	return (1);
}

void	free_frame(t_man *man)
{
	int	i;

	if (man->frame.img)
		mlx_image_destroy(man->xvar, man->frame.img);
	man->frame.img = 0;
	i = 0;
	while (i < 2)
	{
		if (man->swap_buf[i].img)
			mlx_image_destroy(man->xvar, man->swap_buf[i].img);
		man->swap_buf[i].img = 0;
		++i;
	}
	free(man->frame.xmap);
	free(man->frame.ymap);
	man->frame.xmap = 0;
	man->frame.ymap = 0;
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
	f->thickness = (double)f->size.x / man->res.res.x;
	f->img = mlx_image_create(man->xvar, f->size.x, f->size.y);
	if (!f->img)
		return (put_error(man, E_FAIL_MLX_IMG, 0));
	return (1);
}

static int	set_xmap_and_ymap(t_man *man)
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
