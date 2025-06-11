#include "cimmerian.h"

static void	add_ceiling_and_floor_to_skybox(t_map *map);
static void	apply_ceiling_to_curr_cycle_frame(t_img *img, int h_gradient,
				t_color top);
static void	apply_floor_to_curr_cycle_frame(t_img *img, int h_gradient,
				t_color bottom);

void	compose_skybox(t_man *man, t_map *map, t_img *src)
{
	t_ivec2	size;

	if (map->skybox)
	{
		free_image(map->skybox, free);
		map->skybox = 0;
	}
	if (src)
		map->skybox = duplicate_image("skybox", src);
	if (!map->skybox)
	{
		set_ivec2(&size, man->res.res.x * 4, man->res.res.y);
		map->skybox = create_empty_image("skybox", size, 255);
	}
	add_ceiling_and_floor_to_skybox(map);
	return ;
}

static void	add_ceiling_and_floor_to_skybox(t_map *map)
{
	int	h_gradient;

	if (!map->skybox)
		return ;
	h_gradient = map->skybox->size.y / 2;
	while (map->skybox->cycle_index < map->skybox->cycle_len)
	{
		apply_ceiling_to_curr_cycle_frame(map->skybox, h_gradient,
			map->ceil_color);
		apply_floor_to_curr_cycle_frame(map->skybox, h_gradient,
			map->floor_color);
		++map->skybox->cycle_index;
	}
	map->skybox->cycle_index = 0;
	return ;
}

static void	apply_ceiling_to_curr_cycle_frame(t_img *img, int h_gradient,
	t_color top)
{
	t_vert	v;
	double	factor;

	if (!top.a)
		return ;
	v.coord.y = 0;
	while (v.coord.y < img->size.y / 2)
	{
		v.coord.x = 0;
		factor = (double)(v.coord.y) / h_gradient;
		while (v.coord.x < img->size.x)
		{
			v.color.r = (1 - factor) * top.r;
			v.color.g = (1 - factor) * top.g;
			v.color.b = (1 - factor) * top.b;
			v.color.a = 255;
			draw_pixel(img->cycle[img->cycle_index], v.color, v.coord,
				img->size);
			++v.coord.x;
		}
		++v.coord.y;
	}
	return ;
}

static void	apply_floor_to_curr_cycle_frame(t_img *img, int h_gradient,
	t_color bottom)
{
	t_vert	v;
	double	factor;

	if (!bottom.a)
		return ;
	v.coord.y = img->size.y / 2;
	while (v.coord.y < img->size.y)
	{
		v.coord.x = 0;
		factor = (double)(v.coord.y - img->size.y / 2) / h_gradient;
		while (v.coord.x < img->size.x)
		{
			v.color.r = factor * bottom.r;
			v.color.g = factor * bottom.g;
			v.color.b = factor * bottom.b;
			v.color.a = 255;
			draw_pixel(img->cycle[img->cycle_index], v.color, v.coord,
				img->size);
			++v.coord.x;
		}
		++v.coord.y;
	}
	return ;
}
