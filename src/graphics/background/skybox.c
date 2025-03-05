#include "cimmerian.h"

static void	add_floor_to_skybox(t_img *img, t_color fog);
static void	apply_modif_to_curr_cycle_frame(t_img *img, t_color fog,
				int h_gradient, t_color bottom);

void	compose_skybox(t_map *m, t_img *src, t_color fog)
{
	m->skybox = duplicate_image("skybox", src);
	add_floor_to_skybox(m->skybox, fog);
	return ;
}

static void	add_floor_to_skybox(t_img *img, t_color fog)
{
	int		h_gradient;
	t_color	bottom;

	if (!img)
		return ;
	h_gradient = img->size.y / 2;
	bottom = get_color_rgba(42, 30, 30, 255);
	while (img->cycle_index < img->cycle_len)
	{
		apply_modif_to_curr_cycle_frame(img, fog, h_gradient, bottom);
		++img->cycle_index;
	}
	img->cycle_index = 0;
	return ;
}

static void	apply_modif_to_curr_cycle_frame(t_img *img, t_color fog,
	int h_gradient, t_color bottom)
{
	t_vert	v;
	double	factor;

	v.coord.y = img->size.y / 2;
	while (v.coord.y < img->size.y)
	{
		v.coord.x = 0;
		factor = (double)(v.coord.y - img->size.y / 2) / h_gradient;
		while (v.coord.x < img->size.x)
		{
			v.color.r = (1 - factor) * fog.r + factor * bottom.r;
			v.color.g = (1 - factor) * fog.g + factor * bottom.g;
			v.color.b = (1 - factor) * fog.b + factor * bottom.b;
			v.color.a = 255;
			draw_pixel(img->cycle[img->cycle_index], v.color, v.coord,
				img->size);
			++v.coord.x;
		}
		++v.coord.y;
	}
	return ;
}
