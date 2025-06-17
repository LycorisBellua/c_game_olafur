#include "olafur.h"

static void	fix_initial_pos(t_ivec2 *pos);
static void	draw_char(t_man *man, t_img *image, t_ivec2 pos);
static void	alignment_left(const char *str, size_t *i, t_ivec2 *pos);

/*
	The capacity is 90 characters per line ([0-89]).
	The index is the result of `(pos->x - FONT_PAD) / FONT_SIZE_X`.
*/
void	draw_font_default(t_man *man, t_ivec2 *pos, const char *str)
{
	size_t	i;
	size_t	len;
	t_img	*img;

	img = get_image(man, "font_default");
	if (!str || !img)
		return ;
	i = 0;
	len = strlen(str);
	fix_initial_pos(pos);
	while (i <= len)
	{
		img->cycle_index = clamp(str[i] - ' ', 0, img->cycle_len - 1);
		alignment_left(str, &i, pos);
		draw_char(man, img, *pos);
		pos->x += FONT_SIZE_X;
		if (str[i] == '\n')
			set_ivec2(pos, FONT_PAD, pos->y + FONT_SIZE_Y * 2);
		if ((pos->x + FONT_SIZE_X + FONT_PAD) >= man->res.res.x)
			set_ivec2(pos, FONT_PAD, pos->y + FONT_SIZE_Y);
		if ((pos->y + FONT_SIZE_Y + FONT_PAD) >= man->res.res.y)
			break ;
		++i;
	}
	return ;
}

static void	fix_initial_pos(t_ivec2 *pos)
{
	if (pos->x < FONT_PAD)
		pos->x = FONT_PAD;
	if (pos->y < FONT_PAD)
		pos->y = FONT_PAD;
	if (pos->x % FONT_SIZE_X != FONT_MOD_X)
		pos->x -= pos->x % FONT_SIZE_X + FONT_MOD_X;
	if (pos->y % FONT_SIZE_Y != FONT_MOD_Y)
		pos->y -= pos->y % FONT_SIZE_Y + FONT_MOD_Y;
	return ;
}

static void	draw_char(t_man *man, t_img *image, t_ivec2 pos)
{
	size_t	i;
	size_t	len;
	t_ivec2	p;

	i = 0;
	len = image->size.x * image->size.y;
	while (i < len)
	{
		p.y = i / image->size.x;
		p.x = i - p.y * image->size.x;
		set_ivec2(&p, p.x + pos.x, p.y + pos.y);
		draw_point(man, image->cycle[image->cycle_index][i], p.x, p.y);
		++i;
	}
	return ;
}

static void	alignment_left(const char *str, size_t *i, t_ivec2 *pos)
{
	size_t	i_curr_space;
	char	*p_next_space;

	if (str[*i] != ' ')
		return ;
	if (pos->x == FONT_PAD)
	{
		while (str[*i] == ' ')
			++*i;
		return ;
	}
	i_curr_space = (pos->x - FONT_PAD) / FONT_SIZE_X;
	p_next_space = strchr(str + *i + 1, ' ');
	if (p_next_space && i_curr_space + p_next_space - (str + *i) > 89)
		pos->x += (89 - i_curr_space) * FONT_SIZE_X;
	return ;
}
