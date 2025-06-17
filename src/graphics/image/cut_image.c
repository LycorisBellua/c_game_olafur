#include "olafur.h"

static void	cut_image_regular(t_png *file, t_img *img, size_t i_seg,
				size_t i_cyc);
static void	cut_image_shadow(t_png *file, t_img *img, size_t i_seg,
				size_t i_cyc);

void	cut_image(t_png *file, t_img *img, size_t i)
{
	size_t	j;

	j = 0;
	while (j < img->cycle_len)
	{
		cut_image_regular(file, img, i, j);
		cut_image_shadow(file, img, i, j);
		++j;
	}
	return ;
}

static void	cut_image_regular(t_png *file, t_img *img, size_t i_seg,
	size_t i_cyc)
{
	int		i;
	int		line;
	t_ivec2	pos;
	t_color	*file_ptr;
	t_color	*cycle_ptr;

	if (!file->seg[i_seg].cycle)
		return ;
	pos.x = file->seg[i_seg].cycle[i_cyc].x;
	pos.y = file->seg[i_seg].cycle[i_cyc].y;
	file_ptr = 0;
	line = 0;
	while (line < img->size.y)
	{
		cycle_ptr = &img->cycle[i_cyc][img->size.x * line];
		file_ptr = &file->buf[file->size.x * (pos.y + line) + pos.x];
		i = -1;
		while (++i < img->size.x)
			cycle_ptr[i] = file_ptr[i];
		++line;
	}
	return ;
}

static void	cut_image_shadow(t_png *file, t_img *img, size_t i_seg,
	size_t i_cyc)
{
	int		i;
	int		line;
	t_ivec2	pos;
	t_color	*file_ptr;
	t_color	*cycle_ptr;

	if (!file->seg[i_seg].cycle || !img->cycle_shadow)
		return ;
	pos.x = file->seg[i_seg].cycle[i_cyc].x + file->seg[i_seg].shadow_offset.x;
	pos.y = file->seg[i_seg].cycle[i_cyc].y + file->seg[i_seg].shadow_offset.y;
	file_ptr = 0;
	line = 0;
	while (line < img->size.y)
	{
		cycle_ptr = &img->cycle_shadow[i_cyc][img->size.x * line];
		file_ptr = &file->buf_shadow[file->size.x * (pos.y + line) + pos.x];
		i = -1;
		while (++i < img->size.x)
			cycle_ptr[i] = file_ptr[i];
		++line;
	}
	return ;
}
