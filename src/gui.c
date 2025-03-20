#include "cimmerian.h"

static void	display_fps(t_man *man, t_frame *f, t_ivec2 pos);
static void	display_game_over_message(t_man *man, t_frame *f, t_ivec2 *pos);

void	display_game_gui(t_man *man, t_frame *f)
{
	static t_img	*cursor;
	static t_img	*soul_icon;
	t_ivec2			pos;

	if (!cursor)
		cursor = get_image(man, "cursor");
	if (!soul_icon)
		soul_icon = get_image(man, "soul_idle");
	draw_minimap(man, f);
	set_ivec2(&pos, 0, 0);
	draw_image(f, soul_icon, pos);
	pos.x += soul_icon->size.x;
	pos.y += 14;
	display_fps(man, f, pos);
	if (man->cursor.x >= 0 && man->cursor.y >= 0)
		draw_cursor(f, cursor, man->cursor, man->l_click_action);
	return ;
}

void	display_game_over_screen(t_man *man, t_frame *f)
{
	static t_img	*cursor;
	static t_img	*soul_icon;
	t_ivec2			pos;

	fill_frame(f, man->map->fog_color);
	if (!cursor)
		cursor = get_image(man, "cursor");
	if (!soul_icon)
		soul_icon = get_image(man, "soul_idle");
	draw_minimap(man, f);
	set_ivec2(&pos, 0, 0);
	draw_image(f, soul_icon, pos);
	pos.x += soul_icon->size.x;
	pos.y += 14;
	display_fps(man, f, pos);
	display_game_over_message(man, f, &pos);
	if (man->cursor.x >= 0 && man->cursor.y >= 0)
		draw_cursor(f, cursor, man->cursor, man->l_click_action);
	return ;
}

static void	display_fps(t_man *man, t_frame *f, t_ivec2 pos)
{
	static long	dt_ms;
	static int	smoothed_fps;
	char		*s;
	char		*num;

	dt_ms += man->dt_ms;
	if (dt_ms >= 250)
	{
		smoothed_fps = man->fps;
		dt_ms -= 250;
	}
	num = itoa_dec(smoothed_fps);
	s = strjoin("FPS: ", num);
	draw_font_default(man, f, &pos, s);
	free(s);
	free(num);
	return ;
}

static void	display_game_over_message(t_man *man, t_frame *f, t_ivec2 *pos)
{
	char	*msg;

	msg = 0;
	if (man->game_state == GAME_STATE_SUCCESS)
		msg = "You won! :D";
	else if (man->game_state == GAME_STATE_FAILURE)
		msg = "Oh nyo... You ded. :/";
	if (msg)
	{
		pos->x = f->size.x / 2 - strlen(msg) / 2 * FONT_SIZE_X;
		pos->y = f->size.y / 2;
		draw_font_default(man, f, pos, msg);
	}
	return ;
}
