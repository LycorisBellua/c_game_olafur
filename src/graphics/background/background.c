#include "cimmerian.h"

static void	add_fog_overlay(t_img *img, double fog_width, t_color fog);
static void	upper_gradient(t_img *img, t_color fog, int h_gradient, t_vert *v);
static void	lower_gradient(t_img *img, int h_solid, int h_gradient, t_vert *v);

t_img	*compose_background(t_map *m)
{
	t_img	*img;

	img = calloc(1, sizeof(t_img));
	if (!img)
		return (0);
	img->size.x = g_man.res.window_size_default.x;
	img->size.y = g_man.res.window_size_default.y;
	img->buf = calloc(img->size.x * img->size.y * 4, sizeof(GLubyte));
	if (!img->buf)
	{
		free_image(img);
		return (0);
	}
	update_background(m, img);
	return (img);
}

void	update_background(t_map *m, t_img *bg)
{
	size_t	len;

	len = bg->size.x * bg->size.y * 4 * sizeof(GLubyte);
	memcpy(bg->buf, m->skybox->buf, len);
	add_fog_overlay(bg, m->fog_width, m->fog_color);
	return ;
}

static void	add_fog_overlay(t_img *img, double fog_width, t_color fog)
{
	int		h_solid;
	int		h_gradient;
	t_vert	v;

	h_solid = img->size.y / 2 * fog_width;
	h_gradient = img->size.y / 2 - h_solid;
	upper_gradient(img, fog, h_gradient, &v);
	v.color = fog;
	while (v.coord.y < img->size.y / 2 + h_solid)
	{
		v.coord.x = 0;
		while (v.coord.x < img->size.x)
		{
			draw_point_img(img, v.color, v.coord.x, v.coord.y);
			++v.coord.x;
		}
		++v.coord.y;
	}
	lower_gradient(img, h_solid, h_gradient, &v);
	return ;
}

static void	upper_gradient(t_img *img, t_color fog, int h_gradient, t_vert *v)
{
	double	factor;

	v->coord.y = 0;
	while (v->coord.y < h_gradient)
	{
		v->coord.x = 0;
		factor = (double)v->coord.y / h_gradient;
		while (v->coord.x < img->size.x)
		{
			v->color.r = fog.r;
			v->color.g = fog.g;
			v->color.b = fog.b;
			v->color.a = (unsigned char)(factor * fog.a);
			draw_point_img(img, v->color, v->coord.x, v->coord.y);
			++v->coord.x;
		}
		++v->coord.y;
	}
	return ;
}

static void	lower_gradient(t_img *img, int h_solid, int h_gradient, t_vert *v)
{
	t_color	fog;
	double	factor;

	fog = v->color;
	while (v->coord.y < img->size.y)
	{
		v->coord.x = 0;
		factor = (double)(v->coord.y - img->size.y / 2 - h_solid) / h_gradient;
		while (v->coord.x < img->size.x)
		{
			v->color.r = fog.r;
			v->color.g = fog.g;
			v->color.b = fog.b;
			v->color.a = (unsigned char)((1 - factor) * fog.a);
			draw_point_img(img, v->color, v->coord.x, v->coord.y);
			++v->coord.x;
		}
		++v->coord.y;
	}
	return ;
}
