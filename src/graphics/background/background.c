#include "olafur.h"

static void	compose_one_frame(t_man *man, t_map *map, int frame_i, t_png *dst);
static void	copy_skybox_row(t_img *sky, int frame_i, t_png *dst, int y);

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
	int		y;
	t_color	fog;

	fog = map->fog_color;
	if (map->skybox)
	{
		y = -1;
		while (++y < dst->size.y)
			copy_skybox_row(map->skybox, frame_i, dst, y);
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

/* The skybox repeats horizontally, and is stretched vertically. */
static void	copy_skybox_row(t_img *sky, int frame_i, t_png *dst, int y)
{
	int	x;
	int	sky_x;
	int	sky_y;

	sky_y = y * sky->size.y / dst->size.y;
	if (sky_y >= sky->size.y)
		sky_y = sky->size.y - 1;
	sky_x = 0;
	x = 0;
	while (x < dst->size.x)
	{
		dst->buf[y * dst->size.x + x] = sky->cycle[frame_i]
		[sky_x * sky->size.y + sky_y];
		++x;
		if (++sky_x >= sky->size.x)
			sky_x = 0;
	}
	return ;
}
