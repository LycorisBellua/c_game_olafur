#include "cimmerian.h"

static int	set_image_from_segment(t_img *img, t_png *file, size_t i_seg);
static int	allocate_cycles(t_img *img);

int	create_images_from_file(t_man *man, t_png *file, size_t *i_img)
{
	size_t	i;
	t_img	*img;

	i = 0;
	img = &man->images[*i_img];
	while (i < file->segment_len)
	{
		img = &man->images[*i_img];
		if (!set_image_from_segment(img, file, i))
		{
			free_images(man);
			return (0);
		}
		cut_image(file, img, i);
		if (!calculate_image_average_color(img))
		{
			free_images(man);
			return (0);
		}
		++i;
		++*i_img;
	}
	return (1);
}

static int	set_image_from_segment(t_img *img, t_png *file, size_t i_seg)
{
	img->id = strdup(file->seg[i_seg].id);
	if (!img->id)
		return (0);
	set_ivec2(&img->size, file->seg[i_seg].size.x, file->seg[i_seg].size.y);
	img->shadow_offset.x = file->seg[i_seg].shadow_offset.x;
	img->shadow_offset.y = file->seg[i_seg].shadow_offset.y;
	img->still_frame = file->seg[i_seg].still_frame;
	img->cycle_len = file->seg[i_seg].cycle_len;
	img->cycle_index = img->still_frame;
	img->cycle_time_in_ms = file->seg[i_seg].cycle_time_in_ms;
	img->cycle = calloc(img->cycle_len, sizeof(t_color *));
	if (!img->cycle)
		return (0);
	if (file->path_shadow)
	{
		img->cycle_shadow = calloc(img->cycle_len, sizeof(t_color *));
		if (!img->cycle_shadow)
		{
			free(img->cycle);
			img->cycle = 0;
			return (0);
		}
	}
	return (allocate_cycles(img));
}

static int	allocate_cycles(t_img *img)
{
	size_t	i;

	i = 0;
	while (i < img->cycle_len)
	{
		img->cycle[i] = calloc(img->size.x * img->size.y, sizeof(t_color));
		if (!img->cycle[i])
			return (0);
		if (img->cycle_shadow)
		{
			img->cycle_shadow[i] = calloc(img->size.x * img->size.y,
					sizeof(t_color));
			if (!img->cycle_shadow[i])
				return (0);
		}
		++i;
	}
	return (1);
}
