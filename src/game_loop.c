#include "cimmerian.h"

static void	display_gui(t_man *man, t_frame *f);

void	run_game_loop(t_man *man)
{
	t_frame	*f;

	f = man->frame[man->curr_frame];
	update_background(man->map);
	draw_background(man, f);
	raycasting(man, f);
	door_routine(man);
	display_gui(man, f);
	return ;
}

static void	display_gui(t_man *man, t_frame *f)
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
