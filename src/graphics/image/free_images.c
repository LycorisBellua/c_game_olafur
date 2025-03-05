#include "cimmerian.h"

void	free_images(void)
{
	size_t	i;

	i = 0;
	while (i < NBR_IMG)
	{
		free_image(g_man.images + i);
		++i;
	}
	return ;
}

void	free_image(t_img *img)
{
	if (!img)
		return ;
	free(img->id);
	img->cycle_index = 0;
	while (img->cycle_index < img->cycle_len)
	{
		if (img->cycle)
			free(img->cycle[img->cycle_index]);
		if (img->cycle_shadow)
			free(img->cycle_shadow[img->cycle_index]);
		++img->cycle_index;
	}
	free(img->cycle);
	free(img->cycle_shadow);
	free(img->average_color);
	free(img->is_see_through);
	bzero(img, sizeof(t_img));
	return ;
}
