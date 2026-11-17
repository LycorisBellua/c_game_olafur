#include "olafur.h"

static int	segment_item(t_json *j, void *ctx);
static int	segment_member(t_json *j, void *ctx);
static int	parse_cycle(t_json *j, t_png_seg *seg);
static int	cycle_item(t_json *j, void *ctx);

int	json_parse_segments(t_json *j, t_png *file)
{
	free_segments(file);
	if (!json_expect(j, '[', E_JSON_ARRAY))
		return (0);
	return (json_list(j, ']', segment_item, file));
}

static int	segment_item(t_json *j, void *ctx)
{
	t_png		*file;
	t_png_seg	*new_arr;

	file = ctx;
	new_arr = realloc(file->seg, (file->segment_len + 1) * sizeof(t_png_seg));
	if (!new_arr)
		return (put_error(E_FAIL_MEM, 0, 0));
	file->seg = new_arr;
	memset(&file->seg[file->segment_len], 0, sizeof(t_png_seg));
	++file->segment_len;
	if (!json_expect(j, '{', E_JSON_OBJECT))
		return (0);
	return (json_list(j, '}', segment_member, &file->seg[file->segment_len
		- 1]));
}

static int	segment_member(t_json *j, void *ctx)
{
	t_png_seg	*seg;
	char		*key;
	int			ok;

	seg = ctx;
	ok = json_key(j, &key);
	if (ok && key && !strcmp(key, "id"))
		ok = json_string(j, &seg->id);
	else if (ok && key && !strcmp(key, "size"))
		ok = json_ivec2(j, &seg->size);
	else if (ok && key && !strcmp(key, "shadow_offset"))
		ok = json_ivec2(j, &seg->shadow_offset);
	else if (ok && key && !strcmp(key, "still_frame"))
		ok = json_int(j, &seg->still_frame);
	else if (ok && key && !strcmp(key, "cycle_time_in_ms"))
		ok = json_int(j, &seg->cycle_time_in_ms);
	else if (ok && key && !strcmp(key, "cycle"))
		ok = parse_cycle(j, seg);
	else if (ok)
		ok = json_unknown_field(j, key);
	free(key);
	return (ok);
}

static int	parse_cycle(t_json *j, t_png_seg *seg)
{
	free(seg->cycle);
	seg->cycle = 0;
	seg->cycle_len = 0;
	if (!json_expect(j, '[', E_JSON_ARRAY))
		return (0);
	return (json_list(j, ']', cycle_item, seg));
}

static int	cycle_item(t_json *j, void *ctx)
{
	t_png_seg	*seg;
	t_ivec2		*new_arr;

	seg = ctx;
	new_arr = realloc(seg->cycle, (seg->cycle_len + 1) * sizeof(t_ivec2));
	if (!new_arr)
		return (put_error(E_FAIL_MEM, 0, 0));
	seg->cycle = new_arr;
	++seg->cycle_len;
	return (json_ivec2(j, &seg->cycle[seg->cycle_len - 1]));
}
