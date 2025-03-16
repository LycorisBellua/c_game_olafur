#include "cimmerian.h"

void	free_images(t_man *man)
{
	size_t	i;

	i = 0;
	while (i < NBR_IMG)
	{
		free_image(man->images + i, 0);
		++i;
	}
	return ;
}

void	free_image(t_img *img, void (free_fct)(void *))
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
	if (free_fct)
		free(img);
	else
		bzero(img, sizeof(t_img));
	return ;
}
