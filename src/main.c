#include "cimmerian.h"

t_man	g_man;

int	main(int argc, char **argv)
{
	t_ivec2	window_size;

	bzero(&g_man, sizeof(t_man));
	g_man.bonus = 1;
	if (argc == 1)
		return (put_error(0, E_NO_ARG, 0, EXIT_FAILURE));
	else if (argc > 2)
		return (put_error(0, E_TOO_MANY_ARGS, 0, EXIT_FAILURE));
	set_ivec2(&window_size, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!create_window(&g_man, TITLE, window_size, 16.0 / 9)
		|| !init_frame(&g_man) || !set_image_array(&g_man, PATH_IMG_JSON))
		return (EXIT_FAILURE);
	audio_init(&g_man.audio);
	g_man.audio.music = audio_source_create(0);
	init_minimap_values(&g_man);
	init_fog(&g_man);
	g_man.curr_map = add_map(&g_man, argv[1]);
	if (g_man.curr_map < 0)
		return (put_error(&g_man, 0, 0, EXIT_FAILURE));
	reset_player(&g_man);
	init_input_handling(&g_man);
	g_man.game_state = GAME_STATE_PLAY;
	run_game_loop(&g_man);
	deinit(&g_man);
	return (EXIT_SUCCESS);
}
