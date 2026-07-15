#include "olafur.h"

static void	fix_initial_pos(t_ivec2 *pos, int s);
static void	draw_char(t_man *man, t_img *image, t_ivec2 pos);
static void	alignment_left(const char *str, size_t *i, t_ivec2 *pos, int s);

/*
	The capacity is 90 characters per line ([0-89]).
	The index is the result of `(pos->x - FONT_PAD) / FONT_SIZE_X`.

	All font metrics are multiplied by man->gui_scale so the text (and its 
	grid/wrap layout) grows with the resolution while keeping the same logical 
	shape: since the framebuffer and the metrics scale by the same factor, the 
	per-line character count and grid snapping are unchanged.
*/
void	draw_font(t_man *man, t_ivec2 *pos, const char *str)
{
	size_t	i;
	size_t	len;
	int		s;

	if (!man->img_font || !str)
		return ;
	s = man->gui_scale;
	i = 0;
	len = strlen(str);
	fix_initial_pos(pos, s);
	while (i <= len)
	{
		man->img_font->cycle_index = clamp(str[i] - ' ', 0,
			man->img_font->cycle_len - 1);
		alignment_left(str, &i, pos, s);
		draw_char(man, man->img_font, *pos);
		pos->x += FONT_SIZE_X * s;
		if (str[i] == '\n')
			set_ivec2(pos, FONT_PAD * s, pos->y + FONT_SIZE_Y * 2 * s);
		if ((pos->x + (FONT_SIZE_X + FONT_PAD) * s) >= man->res.res.x)
			set_ivec2(pos, FONT_PAD * s, pos->y + FONT_SIZE_Y * s);
		if ((pos->y + (FONT_SIZE_Y + FONT_PAD) * s) >= man->res.res.y)
			break ;
		++i;
	}
	return ;
}

static void	fix_initial_pos(t_ivec2 *pos, int s)
{
	if (pos->x < FONT_PAD * s)
		pos->x = FONT_PAD * s;
	if (pos->y < FONT_PAD * s)
		pos->y = FONT_PAD * s;
	if (pos->x % (FONT_SIZE_X * s) != FONT_MOD_X * s)
		pos->x -= pos->x % (FONT_SIZE_X * s) + FONT_MOD_X * s;
	if (pos->y % (FONT_SIZE_Y * s) != FONT_MOD_Y * s)
		pos->y -= pos->y % (FONT_SIZE_Y * s) + FONT_MOD_Y * s;
	return ;
}

static void	draw_char(t_man *man, t_img *image, t_ivec2 pos)
{
	size_t	i;
	size_t	len;
	t_ivec2	p;
	int		s;

	s = man->gui_scale;
	i = 0;
	len = image->size.x * image->size.y;
	while (i < len)
	{
		p.x = i / image->size.y;
		p.y = i - p.x * image->size.y;
		draw_scaled_point(man, image->cycle[image->cycle_index][i],
			(t_ivec2){pos.x + p.x * s, pos.y + p.y * s}, s);
		++i;
	}
	return ;
}

static void	alignment_left(const char *str, size_t *i, t_ivec2 *pos, int s)
{
	size_t	i_curr_space;
	char	*p_next_space;

	if (str[*i] != ' ')
		return ;
	if (pos->x == FONT_PAD * s)
	{
		while (str[*i] == ' ')
			++*i;
		return ;
	}
	i_curr_space = (pos->x - FONT_PAD * s) / (FONT_SIZE_X * s);
	p_next_space = strchr(str + *i + 1, ' ');
	if (p_next_space && i_curr_space + p_next_space - (str + *i) > 89)
		pos->x += (89 - i_curr_space) * FONT_SIZE_X * s;
	return ;
}
