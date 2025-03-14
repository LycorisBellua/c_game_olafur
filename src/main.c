#include "cimmerian.h"

t_man	g_man;

static void	clear_buffer(t_man *man);
static void	draw_into_buffer(t_man *man);
static void	swap_buffer(t_man *man);

int	main(void)
{
	bzero(&g_man, sizeof(t_man));
	if (!create_window(&g_man, TITLE, RES_WIDTH, RES_HEIGHT))
		return (EXIT_FAILURE);
	if (!create_frames(&g_man))
		return (EXIT_FAILURE);
	if (!set_image_array(&g_man, "img/index.json"))
		return (EXIT_FAILURE);
	if (!create_map(&g_man))
		return (EXIT_FAILURE);
	add_outline_to_font(&g_man.images[1]);
	set_ivec2(&g_man.cursor, -1, -1);
	glfwSetKeyCallback(g_man.window, physical_key_callback);
	glfwSetInputMode(g_man.window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
	glfwSetScrollCallback(g_man.window, scroll_callback);
	glfwSetMouseButtonCallback(g_man.window, mouse_callback);
	glfwSetCursorPosCallback(g_man.window, cursor_pos_callback);
	reset_player_transform(&g_man);
	set_vec2(&g_man.player.pos, 22, 12);
	while (!glfwWindowShouldClose(g_man.window))
	{
		set_dt_and_fps(&g_man);
		advance_all_image_cycles(&g_man);
		clear_buffer(&g_man);
		draw_into_buffer(&g_man);
		swap_buffer(&g_man);
		glfwPollEvents();
		update_player_transform(&g_man);
	}
	deinit(&g_man);
	return (EXIT_SUCCESS);
}

static void	clear_buffer(t_man *man)
{
	/* Keep using glClear for when the window is resized */
	glClear(GL_COLOR_BUFFER_BIT);
	clear_drawing(man->frame[man->curr_frame]);
	return ;
}

static void	draw_into_buffer(t_man *man)
{
	run_game_loop(man);
	save_drawing(man->frame[man->curr_frame]);
	render_mesh();
	return ;
}

static void	swap_buffer(t_man *man)
{
	/*
		Keep using glfwSwapBuffers or the viewport remains black.
		It's the equivalent to mlx_put_image_to_window
	*/
	man->curr_frame = (man->curr_frame + 1) % 2;
	glfwSwapBuffers(g_man.window);
	return ;
}
