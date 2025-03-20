#include "cimmerian.h"

static void	goal_routine(t_man *man);

int	game_loop(t_man *man)
{
	t_frame	*f;

	f = man->frame + man->curr_frame;
	set_dt_and_fps(man);
	advance_all_image_cycles(man);
	clear_frame(f);
	if (man->game_state == GAME_STATE_PLAY)
		render_game(man, f);
	else
		display_game_over_screen(man, f);
	display_frame(man, f);
	man->curr_frame = (man->curr_frame + 1) % 2;
	if (man->game_state == GAME_STATE_PLAY)
	{
		update_player_transform(man);
		door_routine(man);
		goal_routine(man);
	}
	poll_input_events(man);
	return (0);
}

static void	goal_routine(t_man *man)
{
	t_ivec2	pos;

	set_ivec2(&pos, (int)man->player.pos.x, (int)man->player.pos.y);
	if (man->map->cells[pos.y * man->map->size.x + pos.x].is_goal)
		man->game_state = GAME_STATE_SUCCESS;
	return ;
}
