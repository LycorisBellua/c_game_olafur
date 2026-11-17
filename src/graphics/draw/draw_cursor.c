#include "olafur.h"

void	draw_cursor(t_man *man, t_img *img, t_ivec2 p, int cyc)
{
	if (!img)
		return ;
	img->cycle_index = cyc % img->cycle_len;
	draw_image_scaled(man, img, p, 2 * man->gui_scale);
	return ;
}
