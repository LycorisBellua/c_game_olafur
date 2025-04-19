#include "cimmerian.h"

static void	init_fog(t_man *man);

t_man	g_man;

int	main(int argc, char **argv)
{
	if (argc != 2)
		put_error_and_exit(NULL, "", EXIT_FAILURE);
	bzero(&g_man, sizeof(t_man));
	if (!create_window(&g_man, TITLE, RES_WIDTH, RES_HEIGHT))
		put_error_and_exit(NULL, "", EXIT_FAILURE);
	init_frames(&g_man);
	set_image_array(&g_man, "img/index.json");
	init_minimap_values(&g_man);
	init_fog(&g_man);
	add_map(&g_man, "null");
	add_map(&g_man, argv[1]);
	g_man.curr_map = 1;
	reset_player_transform(&g_man);
	init_input_handling(&g_man);
	g_man.game_state = GAME_STATE_PLAY;
	run_game_loop(&g_man);
	return (EXIT_SUCCESS);
}

void	put_error_and_exit(t_man *man, char *msg, int errno)
{
	deinit(man);
	if (msg)
		dprintf(2, "Error: %s\n", msg);
	exit(errno);
	return ;
}

static void	init_fog(t_man *man)
{
	man->echolocation = ECHOLOCATION;
	if (!man->echolocation)
		man->dof = DEFAULT_DOF;
	man->fog_width = get_fog_width(man->dof);
	return ;
}
