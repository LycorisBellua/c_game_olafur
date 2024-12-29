#include "cimmerian.h"

static int	set_sprite_from_segment(t_spr *s, t_img *file, size_t i_seg);
static int	allocate_cycles(t_spr *s);
static void	cut_sprite(t_img *file, t_spr *s, size_t i_seg, size_t i_cyc);
static void	cut_sprite_shadow(t_img *file, t_spr *s, size_t i_seg,
				size_t i_cyc);

int	create_sprites_from_file(t_img *file, size_t *i_spr)
{
	size_t	i;
	size_t	j;
	t_spr	*s;

	i = 0;
	s = &g_man.sprites[*i_spr];
	while (i < file->segment_len)
	{
		s = &g_man.sprites[*i_spr];
		if (!set_sprite_from_segment(s, file, i))
		{
			free_sprites();
			return (0);
		}
		j = 0;
		while (j < s->cycle_len)
		{
			cut_sprite(file, s, i, j);
			cut_sprite_shadow(file, s, i, j);
			++j;
		}
		++i;
		++*i_spr;
	}
	return (1);
}

static int	set_sprite_from_segment(t_spr *s, t_img *file, size_t i_seg)
{
	s->id = strdup(file->seg[i_seg].id);
	if (!s->id)
		return (0);
	set_ivec2(&s->size, file->seg[i_seg].size.x, file->seg[i_seg].size.y);
	s->shadow_offset.x = file->seg[i_seg].shadow_offset.x;
	s->shadow_offset.y = file->seg[i_seg].shadow_offset.y;
	s->still_frame = file->seg[i_seg].still_frame;
	s->cycle_time_in_ms = file->seg[i_seg].cycle_time_in_ms;
	s->cycle_len = file->seg[i_seg].cycle_len;
	s->cycle = calloc(s->cycle_len, sizeof(t_color *));
	if (!s->cycle)
		return (0);
	if (file->path_shadow)
	{
		s->cycle_shadow = calloc(s->cycle_len, sizeof(t_color *));
		if (!s->cycle_shadow)
		{
			free(s->cycle);
			s->cycle = 0;
			return (0);
		}
	}
	return (allocate_cycles(s));
}

static int	allocate_cycles(t_spr *s)
{
	size_t	i;

	i = 0;
	while (i < s->cycle_len)
	{
		s->cycle[i] = calloc(s->size.x * s->size.y, sizeof(t_color));
		if (!s->cycle[i])
			return (0);
		if (s->cycle_shadow)
		{
			s->cycle_shadow[i] = calloc(s->size.x * s->size.y, sizeof(t_color));
			if (!s->cycle_shadow[i])
				return (0);
		}
		++i;
	}
	return (1);
}

static void	cut_sprite(t_img *file, t_spr *s, size_t i_seg, size_t i_cyc)
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
	while (line < s->size.y)
	{
		cycle_ptr = &s->cycle[i_cyc][s->size.x * line];
		file_ptr = &file->buf[file->size.x * (pos.y + line) + pos.x];
		i = -1;
		while (++i < s->size.x)
			cycle_ptr[i] = file_ptr[i];
		++line;
	}
	return ;
}

static void	cut_sprite_shadow(t_img *file, t_spr *s, size_t i_seg, size_t i_cyc)
{
	int		i;
	int		line;
	t_ivec2	pos;
	t_color	*file_ptr;
	t_color	*cycle_ptr;

	if (!file->seg[i_seg].cycle || !s->cycle_shadow)
		return ;
	pos.x = file->seg[i_seg].cycle[i_cyc].x + file->seg[i_seg].shadow_offset.x;
	pos.y = file->seg[i_seg].cycle[i_cyc].y + file->seg[i_seg].shadow_offset.y;
	file_ptr = 0;
	line = 0;
	while (line < s->size.y)
	{
		cycle_ptr = &s->cycle_shadow[i_cyc][s->size.x * line];
		file_ptr = &file->buf_shadow[file->size.x * (pos.y + line) + pos.x];
		i = -1;
		while (++i < s->size.x)
			cycle_ptr[i] = file_ptr[i];
		++line;
	}
	return ;
}
