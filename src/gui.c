#include "cimmerian.h"

static void	display_collectibles(t_man *man, t_frame *f);
static void	display_centered_message(t_man *man, t_frame *f, t_ivec2 *pos,
				char *msg);

void	display_game_gui(t_man *man, t_frame *f)
{
	static t_img	*cursor;

	draw_minimap(man, f, man->maps[man->curr_map]);
	display_collectibles(man, f);
	if (man->show_debug)
		display_fps(man, f);
	if (!cursor)
		cursor = get_image(man, "cursor");
	if (man->cursor.x >= 0 && man->cursor.y >= 0)
		draw_cursor(f, cursor, man->cursor, man->l_click_action);
	return ;
}

void	display_game_over_screen(t_man *man, t_frame *f)
{
	static t_img	*cursor;
	t_ivec2			pos;
	char			*msg;

	msg = 0;
	if (man->game_state == GAME_STATE_SUCCESS)
		msg = "You won! :D";
	else if (man->game_state == GAME_STATE_FAILURE)
		msg = "Oh nyo... You ded. :/";
	fill_frame(f, man->maps[man->curr_map]->fog_color);
	draw_minimap(man, f, man->maps[man->curr_map]);
	display_collectibles(man, f);
	display_centered_message(man, f, &pos, msg);
	if (!cursor)
		cursor = get_image(man, "cursor");
	if (man->cursor.x >= 0 && man->cursor.y >= 0)
		draw_cursor(f, cursor, man->cursor, man->l_click_action);
	return ;
}

static void	display_collectibles(t_man *man, t_frame *f)
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
	draw_image(f, soul_icon, pos);
	if (soul_icon)
		set_ivec2(&pos, pos.x + soul_icon->size.x, pos.y + 14);
	tmp1 = itoa(man->player.collected);
	tmp2 = strjoin(tmp1, "/");
	free(tmp1);
	tmp1 = itoa(man->player.to_collect);
	tmp3 = strjoin(tmp2, tmp1);
	free(tmp1);
	free(tmp2);
	draw_font_default(man, f, &pos, tmp3);
	free(tmp3);
	return ;
}

static void	display_centered_message(t_man *man, t_frame *f, t_ivec2 *pos,
	char *msg)
{
	if (!msg || !pos)
		return ;
	pos->x = f->size.x / 2 - strlen(msg) / 2 * FONT_SIZE_X;
	pos->y = f->size.y / 2;
	draw_font_default(man, f, pos, msg);
	return ;
}
