#include "olafur.h"

static int	duplicate_cycles(t_img *dst, t_img *src);
static int	duplicate_average_color_and_is_see_through(t_img *dst, t_img *src);

t_img	*duplicate_image(const char *dst_id, t_img *src)
{
	t_img	*dst;

	if (!dst_id || !src)
		return (0);
	dst = calloc(1, sizeof(t_img));
	if (!dst)
		return (0);
	dst->id = strdup(dst_id);
	if (!dst->id)
	{
		free_image(dst, free);
		return (0);
	}
	set_ivec2(&dst->size, src->size.x, src->size.y);
	set_ivec2(&dst->shadow_offset, src->shadow_offset.x, src->shadow_offset.y);
	dst->still_frame = src->still_frame;
	dst->cycle_len = src->cycle_len;
	dst->cycle_time_in_ms = src->cycle_time_in_ms;
	if (!duplicate_cycles(dst, src)
		|| !duplicate_average_color_and_is_see_through(dst, src))
	{
		free_image(dst, free);
		return (0);
	}
	return (dst);
}

static int	duplicate_cycles(t_img *dst, t_img *src)
{
	size_t	i;

	i = 0;
	dst->cycle = calloc(dst->cycle_len, sizeof(t_color *));
	if (src->cycle_shadow)
		dst->cycle_shadow = calloc(dst->cycle_len, sizeof(t_color *));
	if (!dst->cycle || (src->cycle_shadow && !dst->cycle_shadow))
		return (0);
	while (i < dst->cycle_len)
	{
		dst->cycle[i] = calloc(dst->size.x * dst->size.y, sizeof(t_color));
		if (src->cycle_shadow)
			dst->cycle_shadow[i] = calloc(dst->size.x * dst->size.y,
					sizeof(t_color));
		if (!dst->cycle[i] || (src->cycle_shadow && !dst->cycle_shadow[i]))
			return (0);
		memcpy(dst->cycle[i], src->cycle[i],
			dst->size.x * dst->size.y * sizeof(t_color));
		if (src->cycle_shadow)
			memcpy(dst->cycle_shadow[i], src->cycle_shadow[i],
				dst->size.x * dst->size.y * sizeof(t_color));
		++i;
	}
	return (1);
}

static int	duplicate_average_color_and_is_see_through(t_img *dst, t_img *src)
{
	dst->average_color = calloc(dst->cycle_len, sizeof(t_color));
	if (!dst->average_color)
		return (0);
	memcpy(dst->average_color, src->average_color,
		dst->cycle_len * sizeof(t_color));
	dst->is_see_through = calloc(dst->cycle_len, sizeof(int));
	if (!dst->is_see_through)
		return (0);
	memcpy(dst->is_see_through, src->is_see_through,
		dst->cycle_len * sizeof(int));
	return (1);
}
