#include "olafur.h"

static void	fix_initial_pos(t_ivec2 *pos, int s);
static void	advance_pos(t_man *man, t_ivec2 *pos, char c);
static void	alignment_left(t_man *man, const char *str, size_t *i,
				t_ivec2 *pos);

void	draw_font(t_man *man, t_ivec2 *pos, const char *str)
{
	size_t	i;
	size_t	len;
	int		s;
	t_img	*font;

	font = man->img_font;
	if (!font || !str)
		return ;
	s = man->gui_scale;
	i = 0;
	len = strlen(str);
	fix_initial_pos(pos, s);
	while (i < len)
	{
		alignment_left(man, str, &i, pos);
		font->cycle_index = clamp(str[i] - ' ', 0, font->cycle_len - 1);
		draw_image(man, font, *pos);
		advance_pos(man, pos, str[i]);
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
	pos->x -= (pos->x - FONT_PAD * s) % (FONT_SIZE_X * s);
	return ;
}

static void	advance_pos(t_man *man, t_ivec2 *pos, char c)
{
	int	s;

	s = man->gui_scale;
	pos->x += FONT_SIZE_X * s;
	if (c == '\n')
		set_ivec2(pos, FONT_PAD * s, pos->y + FONT_SIZE_Y * 2 * s);
	if ((pos->x + (FONT_SIZE_X + FONT_PAD) * s) >= man->res.res.x)
		set_ivec2(pos, FONT_PAD * s, pos->y + FONT_SIZE_Y * s);
	return ;
}

static void	alignment_left(t_man *man, const char *str, size_t *i,
	t_ivec2 *pos)
{
	int			s;
	int			i_curr;
	int			i_last;
	const char	*p_next_space;

	s = man->gui_scale;
	if (str[*i] != ' ')
		return ;
	if (pos->x == FONT_PAD * s)
	{
		while (str[*i] == ' ')
			++*i;
		return ;
	}
	i_curr = (pos->x - FONT_PAD * s) / (FONT_SIZE_X * s);
	i_last = (man->res.res.x - (FONT_SIZE_X + FONT_PAD * 2) * s - 1)
		/ (FONT_SIZE_X * s);
	p_next_space = strchr(str + *i + 1, ' ');
	if (!p_next_space)
		p_next_space = str + strlen(str);
	if (i_curr + p_next_space - (str + *i) - 1 > i_last)
		pos->x += (i_last - i_curr) * FONT_SIZE_X * s;
	return ;
}
