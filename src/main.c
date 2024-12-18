#include "cimmerian.h"

t_manager	g_man;

static GLFWwindow	*init(const char *title);
static void			clear_buffer(void);
static void			draw_into_buffer(t_map *m);
static void			swap_buffer(GLFWwindow *window);
static void			deinit(void);

int	main(void)
{
	GLFWwindow	*window;

	window = init("Cimmerian");
	if (!window)
		return (EXIT_FAILURE);
	glfwSetKeyCallback(window, physical_key_callback);
	glfwSetInputMode(window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	reset_player_transform(g_man.map);
	g_man.player.pos.x = 22;
	g_man.player.pos.y = 12;
	while (!glfwWindowShouldClose(window))
	{
		set_dt_and_fps();
		//printf("FPS = %d\n", g_man.fps);
		clear_buffer();
		draw_into_buffer(g_man.map);
		swap_buffer(window);
		glfwPollEvents();
		update_player_transform(g_man.map);
	}
	deinit();
	return (EXIT_SUCCESS);
}

static GLFWwindow	*init(const char *title)
{
	GLFWwindow	*window;

	bzero(&g_man, sizeof(t_manager));
	//if (!set_sprite_array("img/sprites/index.json"))
	//	return (0);
	window = get_window(title);
	g_man.shader_program = create_shader_program();
	if (!window || !g_man.shader_program || !create_uniform() || !create_mesh()
		|| !create_frames() || !initialize_maps())
		return (0);
	//add_outline_to_font(&g_man.map->sprites[2]);
	g_man.tex_in_dof = 1.0; // [0.0 - 1.0] percentage
	use_frame(g_man.frame[g_man.curr_frame]);
	return (window);
}

static void	clear_buffer(void)
{
	/* Keep using glClear for when the window is resized */
	glClear(GL_COLOR_BUFFER_BIT);
	clear_drawing(g_man.frame[g_man.curr_frame]);
	return ;
}

static void	draw_into_buffer(t_map *m)
{
	run_game_loop(m);
	save_drawing(g_man.frame[g_man.curr_frame]);
	render_mesh();
	return ;
}

static void	swap_buffer(GLFWwindow *window)
{
	/*
		Keep using glfwSwapBuffers or the viewport remains black.
		It's the equivalent to mlx_put_image_to_window
	*/
	g_man.curr_frame = (g_man.curr_frame + 1) % 2;
	glfwSwapBuffers(window);
	return ;
}

static void	deinit(void)
{
	glfwTerminate();
	free_shader_program();
	free_uniform();
	free_mesh();
	free_frames();
	free_sprites();
	free_maps();
	return ;
}
