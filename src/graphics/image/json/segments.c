#include "cimmerian.h"

static size_t	get_segment_len(char **lines, size_t start, size_t end);
static void		fill_seg_array(t_img *file, char **lines, size_t start,
					size_t end);
static void		parse_cycle(t_img_seg *seg, char **lines, size_t *start,
					size_t end);

void	parse_segments(t_img *file, char **lines, size_t *i)
{
	size_t	start;
	size_t	end;

	start = *i;
	while (lines[*i])
	{
		++*i;
		if (lines[*i - 1][0] == '}' && lines[*i] && lines[*i][0] == ']')
			break ;
	}
	if (!lines[*i])
		--*i;
	end = *i;
	file->segment_len = get_segment_len(lines, start, end);
	file->seg = calloc(file->segment_len, sizeof(t_img_seg));
	if (file->seg)
		fill_seg_array(file, lines, start, end);
	return ;
}

static size_t	get_segment_len(char **lines, size_t start, size_t end)
{
	size_t	len;

	len = 0;
	while (lines[start] && start <= end)
	{
		if (lines[start][0] == '}')
			++len;
		++start;
	}
	return (len);
}

static void	fill_seg_array(t_img *file, char **lines, size_t start, size_t end)
{
	size_t	i_seg;

	i_seg = 0;
	while (lines[start] && start <= end)
	{
		if (lines[start][0] == '}')
			++i_seg;
		else if (is_field(lines[start], "id"))
			file->seg[i_seg].id = get_string_value(lines[start]);
		else if (is_field(lines[start], "size"))
			file->seg[i_seg].size = get_ivec2_value(lines[start]);
		else if (is_field(lines[start], "shadow_offset"))
			file->seg[i_seg].shadow_offset = get_ivec2_value(lines[start]);
		else if (is_field(lines[start], "still_frame"))
			file->seg[i_seg].still_frame = get_int_value(lines[start]);
		else if (is_field(lines[start], "cycle_time_in_ms"))
			file->seg[i_seg].cycle_time_in_ms = get_int_value(lines[start]);
		else if (is_field(lines[start], "cycle"))
			parse_cycle(file->seg + i_seg, lines, &start, end);
		++start;
	}
	return ;
}

static void	parse_cycle(t_img_seg *seg, char **lines, size_t *start, size_t end)
{
	size_t	i;

	i = *start;
	while (lines[i] && i <= end)
	{
		if (lines[i][0] == '[')
			++seg->cycle_len;
		else if (lines[i][0] == ']')
			end = i;
		++i;
	}
	seg->cycle = calloc(seg->cycle_len, sizeof(t_ivec2));
	if (!seg->cycle)
		return ;
	i = 0;
	while (lines[*start] && *start < end)
	{
		if (lines[*start][0] == '[')
			seg->cycle[i++] = get_ivec2_value(lines[*start]);
		++*start;
	}
	return ;
}
