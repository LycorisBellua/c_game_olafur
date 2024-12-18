#include "cimmerian.h"

static void	add_floor(t_img *img, t_color fog);

t_img	*compose_skybox(const char *path_skybox, t_color fog)
{
	t_img	*skybox;

	skybox = load_image_from_file(path_skybox);
	if (!skybox)
		return (0);
	add_floor(skybox, fog);
	return (skybox);
}

static void	add_floor(t_img *img, t_color fog)
{
	int		h_gradient;
	double	factor;
	t_color	bottom;
	t_vert	v;

	h_gradient = img->size.y / 2;
	bottom = get_color_rgba(42, 30, 30, 255);
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
			draw_point_img(img, v.color, v.coord.x, v.coord.y);
			++v.coord.x;
		}
		++v.coord.y;
	}
	return ;
}
