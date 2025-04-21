#include "cimmerian.h"

t_man	g_man;

int	main(int argc, char **argv)
{
	bzero(&g_man, sizeof(t_man));
	if (argc == 1)
		return (put_error(0, E_NO_ARG, EXIT_FAILURE));
	else if (argc > 2)
		return (put_error(0, E_TOO_MANY_ARGS, EXIT_FAILURE));
	else if (!create_window(&g_man, TITLE, WINDOW_WIDTH, WINDOW_HEIGHT)
		|| !init_frames(&g_man) || !set_image_array(&g_man, PATH_IMG_JSON))
		return (EXIT_FAILURE);
	init_minimap_values(&g_man);
	init_fog(&g_man);
	add_map(&g_man, "null");
	g_man.curr_map = add_map(&g_man, argv[1]);
	if (g_man.curr_map < 0)
		return (put_error(&g_man, 0, EXIT_FAILURE));
	reset_player_transform(&g_man);
	init_input_handling(&g_man);
	g_man.game_state = GAME_STATE_PLAY;
	run_game_loop(&g_man);
	return (EXIT_SUCCESS);
}
