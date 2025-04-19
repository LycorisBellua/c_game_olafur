#include "cimmerian.h"

static void	advance_image_cycle(t_img *image, long dt_ms);

void	advance_all_image_cycles(t_man *man)
{
	size_t	i;

	if (man->images)
	{
		i = 0;
		while (man->images[i])
		{
			advance_image_cycle(man->images[i], man->dt_ms);
			++i;
		}
	}
	if (man->maps[man->curr_map] && man->maps[man->curr_map]->skybox)
		advance_image_cycle(man->maps[man->curr_map]->skybox, man->dt_ms);
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
