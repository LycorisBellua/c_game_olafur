#include "cimmerian.h"

static void	advance_image_cycle(t_img *image, long dt_ms);

void	advance_all_image_cycles(long dt_ms)
{
	size_t	i;

	i = 0;
	while (g_man.images[i].id)
	{
		advance_image_cycle(g_man.images + i, dt_ms);
		++i;
	}
	advance_image_cycle(g_man.map->skybox, dt_ms);
	return ;
}

static void	advance_image_cycle(t_img *image, long dt_ms)
{
	if (!image || !dt_ms || !image->cycle_time_in_ms)
		return ;
	image->elapsed_time_in_ms += dt_ms;
	while (image->elapsed_time_in_ms >= image->cycle_time_in_ms)
	{
		image->cycle_index = (image->cycle_index + 1) % image->cycle_len;
		image->elapsed_time_in_ms -= image->cycle_time_in_ms;
	}
	return ;
}
