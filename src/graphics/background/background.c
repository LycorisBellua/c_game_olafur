#include "olafur.h"

static void	add_fog_overlay(t_png *png, float fog_width, t_color fog);
static void	upper_gradient(t_png *png, t_color fog, int h_gradient,
				t_vert *v);
static void	lower_gradient(t_png *png, int h_solid, int h_gradient,
				t_vert *v);

void	compose_background(t_man *man, t_map *map)
{
	t_ivec2	size;
	int		min_len;

	if (!map)
		return ;
	if (!map->background)
	{
		set_ivec2(&size, man->res.res.x * 4, man->res.res.y);
		map->background = create_empty_png(size, 255);
		if (!map->background)
			return ;
	}
	if (map->skybox)
	{
		min_len = min(map->skybox->size.x * map->skybox->size.y,
				map->background->size.x * map->background->size.y);
		memcpy(map->background->buf,
			map->skybox->cycle[map->skybox->cycle_index],
			min_len * sizeof(t_color));
	}
	if (map->skybox)
		map->fog_color = map->skybox->average_color[map->skybox->cycle_index];
	add_fog_overlay(map->background, man->fog_width, map->fog_color);
	return ;
}

static void	add_fog_overlay(t_png *png, float fog_width, t_color fog)
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
	float	factor;

	v->coord.y = 0;
	while (v->coord.y < h_gradient)
	{
		v->coord.x = 0;
		factor = (float)v->coord.y / h_gradient;
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
	float	factor;

	fog = v->color;
	while (v->coord.y < png->size.y)
	{
		v->coord.x = 0;
		factor = (float)(v->coord.y - png->size.y / 2 - h_solid) / h_gradient;
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
