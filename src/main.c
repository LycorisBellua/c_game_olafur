#include "cimmerian.h"

t_man	g_man;

static GLFWwindow	*init(t_man *man, const char *title);
static void			clear_buffer(t_man *man);
static void			draw_into_buffer(t_man *man);
static void			swap_buffer(t_man *man, GLFWwindow *window);
static void			deinit(t_man *man);

int	main(void)
{
	GLFWwindow	*window;

	window = init(&g_man, TITLE);
	if (!window)
		return (EXIT_FAILURE);
	glfwSetKeyCallback(window, physical_key_callback);
	glfwSetInputMode(window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	reset_player_transform(&g_man);
	set_vec2(&g_man.player.pos, 22, 12);
	while (!glfwWindowShouldClose(window))
	{
		set_dt_and_fps(&g_man);
		advance_all_image_cycles(&g_man);
		clear_buffer(&g_man);
		draw_into_buffer(&g_man);
		swap_buffer(&g_man, window);
		glfwPollEvents();
		update_player_transform(&g_man);
	}
	deinit(&g_man);
	return (EXIT_SUCCESS);
}

static GLFWwindow	*init(t_man *man, const char *title)
{
	GLFWwindow	*window;

	bzero(man, sizeof(t_man));
	if (!set_image_array(man, "img/index.json"))
		return (0);
	window = get_window(man, title);
	man->shader_program = create_shader_program();
	if (!window || !man->shader_program || !create_uniform(man)
		|| !create_mesh() || !create_frames(man) || !create_map(man))
		return (0);
	add_outline_to_font(&man->images[1]);
	set_ivec2(&man->cursor, -1, -1);
	use_frame(man->frame[man->curr_frame]);
	return (window);
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

static void	swap_buffer(t_man *man, GLFWwindow *window)
{
	/*
		Keep using glfwSwapBuffers or the viewport remains black.
		It's the equivalent to mlx_put_image_to_window
	*/
	man->curr_frame = (man->curr_frame + 1) % 2;
	glfwSwapBuffers(window);
	return ;
}

static void	deinit(t_man *man)
{
	glfwTerminate();
	free_shader_program(man);
	free_uniform(man);
	free_mesh();
	free_frames(man);
	free_images(man);
	free_map(man);
	return ;
}
