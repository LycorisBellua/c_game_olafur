#include "cimmerian.h"

static void	add_fog_overlay(t_png *png, double fog_width, t_color fog);
static void	upper_gradient(t_png *png, t_color fog, int h_gradient, t_vert *v);
static void	lower_gradient(t_png *png, int h_solid, int h_gradient, t_vert *v);

void	update_background(t_map *m)
{
	memcpy(m->background->buf, m->skybox->cycle[m->skybox->cycle_index],
		m->background->size.x * m->background->size.y * sizeof(t_color));
	add_fog_overlay(m->background, m->fog_width, m->fog_color);
	return ;
}

static void	add_fog_overlay(t_png *png, double fog_width, t_color fog)
{
	int		h_solid;
	int		h_gradient;
	t_vert	v;

	h_solid = png->size.y / 2 * fog_width;
	h_gradient = png->size.y / 2 - h_solid;
	upper_gradient(png, fog, h_gradient, &v);
	v.color = fog;
	while (v.coord.y < png->size.y / 2 + h_solid)
	{
		v.coord.x = 0;
		while (v.coord.x < png->size.x)
		{
			draw_pixel(png->buf, v.color, v.coord, png->size);
			++v.coord.x;
		}
		++v.coord.y;
	}
	lower_gradient(png, h_solid, h_gradient, &v);
	return ;
}

static void	upper_gradient(t_png *png, t_color fog, int h_gradient, t_vert *v)
{
	double	factor;

	v->coord.y = 0;
	while (v->coord.y < h_gradient)
	{
		v->coord.x = 0;
		factor = (double)v->coord.y / h_gradient;
		while (v->coord.x < png->size.x)
		{
			v->color.r = fog.r;
			v->color.g = fog.g;
			v->color.b = fog.b;
			v->color.a = (unsigned char)(factor * fog.a);
			draw_pixel(png->buf, v->color, v->coord, png->size);
			++v->coord.x;
		}
		++v->coord.y;
	}
	return ;
}

static void	lower_gradient(t_png *png, int h_solid, int h_gradient, t_vert *v)
{
	t_color	fog;
	double	factor;

	fog = v->color;
	while (v->coord.y < png->size.y)
	{
		v->coord.x = 0;
		factor = (double)(v->coord.y - png->size.y / 2 - h_solid) / h_gradient;
		while (v->coord.x < png->size.x)
		{
			v->color.r = fog.r;
			v->color.g = fog.g;
			v->color.b = fog.b;
			v->color.a = (unsigned char)((1 - factor) * fog.a);
			draw_pixel(png->buf, v->color, v->coord, png->size);
			++v->coord.x;
		}
		++v->coord.y;
	}
	return ;
}
