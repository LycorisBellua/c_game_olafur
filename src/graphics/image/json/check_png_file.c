#include "olafur.h"

static int	check_segment(t_png *file, t_png_seg *seg);
static int	check_segment_fields(t_png *file, t_png_seg *seg);
static int	is_area_inside(t_png *file, t_ivec2 pos, t_ivec2 size);

/*
	Runs once the pixels of a PNG file object are loaded, so that cutting
	images out of it can never read outside of its buffers.
*/
int	check_png_segments(t_png *file)
{
	size_t	i;

	if (!file->seg || !file->segment_len)
		return (put_error(E_IMG_NO_SEG, file->path, 0));
	i = 0;
	while (i < file->segment_len)
	{
		if (!check_segment(file, &file->seg[i]))
			return (0);
		++i;
	}
	return (1);
}

static int	check_segment(t_png *file, t_png_seg *seg)
{
	size_t	i;
	t_ivec2	pos;

	if (!check_segment_fields(file, seg))
		return (0);
	i = 0;
	while (i < seg->cycle_len)
	{
		pos = seg->cycle[i];
		if (!is_area_inside(file, pos, seg->size))
			return (put_error(E_IMG_OUT_OF_BOUNDS, seg->id, 0));
		set_ivec2(&pos, pos.x + seg->shadow_offset.x,
			pos.y + seg->shadow_offset.y);
		if (file->buf_shadow && !is_area_inside(file, pos, seg->size))
			return (put_error(E_IMG_OUT_OF_BOUNDS, seg->id, 0));
		++i;
	}
	return (1);
}

static int	check_segment_fields(t_png *file, t_png_seg *seg)
{
	if (!seg->id)
		return (put_error(E_IMG_NO_ID, file->path, 0));
	seg->size.x = clamp(seg->size.x, 0, file->size.x);
	seg->size.y = clamp(seg->size.y, 0, file->size.y);
	if (!seg->size.x || !seg->size.y)
		return (put_error(E_IMG_BAD_SIZE, seg->id, 0));
	if (!seg->cycle || !seg->cycle_len)
		return (put_error(E_IMG_NO_CYCLE, seg->id, 0));
	if (seg->still_frame < 0 || (size_t)seg->still_frame >= seg->cycle_len)
		return (put_error(E_IMG_BAD_STILL, seg->id, 0));
	return (1);
}

static int	is_area_inside(t_png *file, t_ivec2 pos, t_ivec2 size)
{
	return (pos.x >= 0 && pos.y >= 0
		&& pos.x + size.x <= file->size.x && pos.y + size.y <= file->size.y);
}
