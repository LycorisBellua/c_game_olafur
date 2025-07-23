#include "olafur.h"

static void	display_collectibles(t_man *man, t_map *map);
static void	display_centered_message(t_man *man, t_ivec2 *pos, const char *msg);

void	display_game_gui(t_man *man, t_map *map)
{
	static t_img	*cursor;
	t_ivec2			pos;

	if (man->game_state == GAME_STATE_SUCCESS)
	{
		fill_frame(man, map->fog_color);
		display_centered_message(man, &pos, "You won! :D");
	}
	else if (man->game_state == GAME_STATE_FAILURE)
	{
		fill_frame(man, map->fog_color);
		display_centered_message(man, &pos, "Oh nyo... You ded. :/");
	}
	draw_minimap(man, map);
	display_collectibles(man, map);
	if (man->show_debug)
		display_fps(man);
	if (!cursor)
		cursor = get_image(man, "cursor");
	if (man->cursor.x >= 0 && man->cursor.y >= 0)
		draw_cursor(man, cursor, man->cursor, man->l_click_action);
	return ;
}

static void	display_collectibles(t_man *man, t_map *map)
{
	static t_img	*soul_icon;
	t_ivec2			pos;
	char			*tmp1;
	char			*tmp2;
	char			*tmp3;

	if (!soul_icon)
		soul_icon = get_image(man, "sprite_soul_idle");
	set_ivec2(&pos, 0, 0);
	draw_image(man, soul_icon, pos);
	if (soul_icon)
		set_ivec2(&pos, pos.x + soul_icon->size.x, pos.y + 14);
	tmp1 = itoa_dec(man->player.collected);
	if (map->to_collect >= 0)
	{
		tmp2 = strjoin(tmp1, "/");
		free(tmp1);
		tmp3 = itoa_dec(map->to_collect);
		tmp1 = strjoin(tmp2, tmp3);
		free(tmp2);
		free(tmp3);
	}
	draw_font_default(man, &pos, tmp1);
	free(tmp1);
	return ;
}

static void	display_centered_message(t_man *man, t_ivec2 *pos, const char *msg)
{
	if (!msg || !pos)
		return ;
	pos->x = man->frame.size.x / 2 - strlen(msg) / 2 * FONT_SIZE_X;
	pos->y = man->frame.size.y / 2;
	draw_font_default(man, pos, msg);
	return ;
}
