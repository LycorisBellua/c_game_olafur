#include "cimmerian.h"

#define PAD    4
#define SIZE_X 7
#define SIZE_Y 12
#define MOD_X  4
#define MOD_Y  4

static void	fix_initial_pos(t_ivec2 *pos);
static void	draw_char(t_frame *frame, t_img *image, t_ivec2 pos);
static void	alignment_left(char *str, size_t *i, t_ivec2 *pos);

/*
	The capacity is 90 characters per line ([0-89]).
	The index is the result of `(pos->x - PAD) / SIZE_X`.
*/
void	draw_font_default(t_frame *frame, t_ivec2 *pos, char *str)
{
	size_t	i;
	size_t	len;
	t_img	*img;

	if (!str)
		return ;
	i = 0;
	len = strlen(str);
	img = &g_man.images[1];
	fix_initial_pos(pos);
	while (i <= len)
	{
		img->cycle_index = clamp(str[i] - ' ', 0, img->cycle_len - 1);
		alignment_left(str, &i, pos);
		draw_char(frame, img, *pos);
		pos->x += SIZE_X;
		if (str[i] == '\n')
			set_ivec2(pos, PAD, pos->y + SIZE_Y * 2);
		if ((pos->x + SIZE_X + PAD) >= g_man.res.window_size_default.x)
			set_ivec2(pos, PAD, pos->y + SIZE_Y);
		if ((pos->y + SIZE_Y + PAD) >= g_man.res.window_size_default.y)
			break ;
		++i;
	}
	return ;
}

static void	fix_initial_pos(t_ivec2 *pos)
{
	if (pos->x < PAD)
		pos->x = PAD;
	if (pos->y < PAD)
		pos->y = PAD;
	if (pos->x % SIZE_X != MOD_X)
		pos->x -= pos->x % SIZE_X + MOD_X;
	if (pos->y % SIZE_Y != MOD_Y)
		pos->y -= pos->y % SIZE_Y + MOD_Y;
	return ;
}

static void	draw_char(t_frame *frame, t_img *image, t_ivec2 pos)
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
		draw_point(frame, image->cycle[image->cycle_index][i], p.x, p.y);
		++i;
	}
	return ;
}

static void	alignment_left(char *str, size_t *i, t_ivec2 *pos)
{
	size_t	i_curr_space;
	char	*p_next_space;

	if (str[*i] != ' ')
		return ;
	if (pos->x == PAD)
	{
		while (str[*i] == ' ')
			++*i;
		return ;
	}
	i_curr_space = (pos->x - PAD) / SIZE_X;
	p_next_space = strchr(str + *i + 1, ' ');
	if (p_next_space && i_curr_space + p_next_space - (str + *i) > 89)
		pos->x += (89 - i_curr_space) * SIZE_X;
	return ;
}
