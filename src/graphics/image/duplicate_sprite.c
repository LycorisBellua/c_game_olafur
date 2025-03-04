#include "cimmerian.h"

t_spr	*duplicate_sprite(const char *dst_id, t_spr *src)
{
	t_spr	*dst;

	if (!dst_id || !src)
		return (0);
	dst = calloc(1, sizeof(t_spr));
	if (!dst)
		return (0);
	dst->id = strdup(dst_id);
	set_ivec2(&dst->size, src->size.x, src->size.y);
	set_ivec2(&dst->shadow_offset, src->shadow_offset.x, src->shadow_offset.y);
	dst->still_frame = src->still_frame;
	dst->cycle_len = src->cycle_len;
	dst->cycle_time_in_ms = src->cycle_time_in_ms;

	size_t i = 0;
	dst->cycle = calloc(dst->cycle_len, sizeof(t_color *));
	while (i < dst->cycle_len)
	{
		dst->cycle[i] = calloc(dst->size.x * dst->size.y, sizeof(t_color));
		if (dst->cycle[i])
			memcpy(dst->cycle[i], src->cycle[i],
					dst->size.x * dst->size.y * sizeof(t_color));
		++i;
	}

	if (src->cycle_shadow)
	{
		dst->cycle_shadow = calloc(dst->cycle_len, sizeof(t_color *));
		i = 0;
		while (i < dst->cycle_len)
		{
			dst->cycle_shadow[i] = calloc(dst->size.x * dst->size.y, sizeof(t_color));
			if (dst->cycle_shadow[i])
				memcpy(dst->cycle_shadow[i], src->cycle_shadow[i],
						dst->size.x * dst->size.y * sizeof(t_color));
			++i;
		}
	}

	dst->average_color = calloc(dst->cycle_len, sizeof(t_color));
	if (dst->average_color)
		memcpy(dst->average_color, src->average_color,
				dst->cycle_len * sizeof(t_color));
	dst->is_see_through = calloc(dst->cycle_len, sizeof(int));
	if (dst->is_see_through)
		memcpy(dst->is_see_through, src->is_see_through,
				dst->cycle_len * sizeof(int));
	/*
		Check if null
		- dst->id
		- dst->cycle (and its individual arrays)
		- dst->cycle_shadow can be null, but if not then check its individual arrays
		- dst->average_color
		- dst->is_see_through
	*/
	return (dst);
}

