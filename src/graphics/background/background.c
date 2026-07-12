#include "olafur.h"

static void	add_fog_overlay(t_png *png, float fog_width, t_color fog);
static void	upper_gradient(t_png *png, t_color fog, int h_gradient,
					t_vert *v);
static void	lower_gradient(t_png *png, int h_solid, int h_gradient,
					t_vert *v);
static void	compose_one_frame(t_man *man, t_map *map, int frame_i, t_png *dst);

void	compose_background(t_man *man, t_map *map)
{
	t_ivec2	size;
	int		count;
	int		i;

	if (!map || map->bg_frames)
		return ;
	count = 1;
	if (map->skybox && map->skybox->cycle_time_in_ms)
		count = map->skybox->cycle_len;
	map->bg_frames = calloc(count, sizeof(t_png *));
	if (!map->bg_frames)
		return ;
	map->bg_frame_count = count;
	set_ivec2(&size, man->res.res.x * 4, man->res.res.y);
	i = -1;
	while (++i < count)
	{
		map->bg_frames[i] = create_empty_png(size, 255);
		if (map->bg_frames[i])
			compose_one_frame(man, map, i, map->bg_frames[i]);
	}
	map->background = map->bg_frames[0];
	if (map->skybox)
		map->fog_color = map->skybox->average_color[0];
	return ;
}

static void	compose_one_frame(t_man *man, t_map *map, int frame_i, t_png *dst)
{
	int		x;
	int		y;
	int		sky_x;
	int		sky_y;
	t_color	fog;

	fog = map->fog_color;
	if (map->skybox)
	{
		y = -1;
		while (++y < dst->size.y)
		{
			sky_y = y * map->skybox->size.y / dst->size.y;
			if (sky_y >= map->skybox->size.y)
				sky_y = map->skybox->size.y - 1;
			sky_x = 0;
			x = 0;
			while (x < dst->size.x)
			{
				dst->buf[y * dst->size.x + x] = map->skybox->cycle[frame_i]
					[sky_x * map->skybox->size.y + sky_y];
				++x;
				if (++sky_x >= map->skybox->size.x)
					sky_x = 0;
			}
		}
		fog = map->skybox->average_color[frame_i];
	}
	add_fog_overlay(dst, man->fog_width, fog);
	return ;
}

void	select_background_frame(t_map *map)
{
	int	idx;

	if (!map || !map->bg_frames)
		return ;
	idx = 0;
	if (map->skybox)
	{
		idx = map->skybox->cycle_index;
		map->fog_color = map->skybox->average_color[idx];
	}
	map->background = map->bg_frames[idx];
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

	if (h_gradient <= 0)
		return ;
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
