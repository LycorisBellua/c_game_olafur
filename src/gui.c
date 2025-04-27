#include "cimmerian.h"

static void	display_collectibles(t_man *man);
static void	display_centered_message(t_man *man, t_ivec2 *pos, char *msg);

void	display_game_gui(t_man *man)
{
	static t_img	*cursor;

	draw_minimap(man, man->maps[man->curr_map]);
	display_collectibles(man);
	if (man->show_debug)
		display_fps(man);
	if (!cursor)
		cursor = get_image(man, "cursor");
	if (man->cursor.x >= 0 && man->cursor.y >= 0)
		draw_cursor(man, cursor, man->cursor, man->l_click_action);
	return ;
}

void	display_game_over_screen(t_man *man)
{
	static t_img	*cursor;
	t_ivec2			pos;
	char			*msg;

	msg = 0;
	if (man->game_state == GAME_STATE_SUCCESS)
		msg = "You won! :D";
	else if (man->game_state == GAME_STATE_FAILURE)
		msg = "Oh nyo... You ded. :/";
	fill_frame(man, man->maps[man->curr_map]->fog_color);
	draw_minimap(man, man->maps[man->curr_map]);
	display_collectibles(man);
	display_centered_message(man, &pos, msg);
	if (man->show_debug)
		display_fps(man);
	if (!cursor)
		cursor = get_image(man, "cursor");
	if (man->cursor.x >= 0 && man->cursor.y >= 0)
		draw_cursor(man, cursor, man->cursor, man->l_click_action);
	return ;
}

static void	display_collectibles(t_man *man)
{
	static t_img	*soul_icon;
	t_ivec2			pos;
	char			*tmp1;
	char			*tmp2;
	char			*tmp3;

	if (!man->player.to_collect)
		return ;
	if (!soul_icon)
		soul_icon = get_image(man, "soul_idle");
	set_ivec2(&pos, 0, 0);
	draw_image(man, soul_icon, pos);
	if (soul_icon)
		set_ivec2(&pos, pos.x + soul_icon->size.x, pos.y + 14);
	tmp1 = itoa(man->player.collected);
	tmp2 = strjoin(tmp1, "/");
	free(tmp1);
	tmp1 = itoa(man->player.to_collect);
	tmp3 = strjoin(tmp2, tmp1);
	free(tmp1);
	free(tmp2);
	draw_font_default(man, &pos, tmp3);
	free(tmp3);
	return ;
}

static void	display_centered_message(t_man *man, t_ivec2 *pos,
	char *msg)
{
	if (!msg || !pos)
		return ;
	pos->x = man->frame.size.x / 2 - strlen(msg) / 2 * FONT_SIZE_X;
	pos->y = man->frame.size.y / 2;
	draw_font_default(man, pos, msg);
	return ;
}
