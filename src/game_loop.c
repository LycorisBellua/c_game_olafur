#include "cimmerian.h"

int	game_loop(t_man *man)
{
	t_frame	*f;

	f = man->frame + man->curr_frame;
	set_dt_and_fps(man);
	advance_all_image_cycles(man);
	clear_frame(f);
	render_game(man, f);
	display_frame(man, f);
	man->curr_frame = (man->curr_frame + 1) % 2;
	handle_input(man);
	door_routine(man);
	return (0);
}
