#include "cimmerian.h"

void			framebuffer_size_callback(GLFWwindow *window, int x, int y);
void			window_pos_callback(GLFWwindow *window, int xpos, int ypos);
int				create_shader_program(void);
int				create_uniform(t_man *man);
int				create_mesh(void);

static int		init_graphics_lib(void);
static void		more_window_settings(t_man *man);
static void		set_initial_viewport(t_man *man);

int	create_window(t_man *man, const char *title, int width, int height)
{
	if (!init_graphics_lib())
		return (0);
	man->title = (char *)title;
	set_resolution(man, width, height);
	man->window = glfwCreateWindow(man->res.window_size.x,
			man->res.window_size.y, man->title, NULL, NULL);
	if (!man->window)
		return (put_error(man, E_FAIL_WINDOW_GL, 0));
	more_window_settings(man);
	if (!init_gl_functions())
		return (put_error(man, E_FAIL_GL_FUNC, 0));
	man->shader_program = create_shader_program();
	if (!man->shader_program || !create_uniform(man) || !create_mesh())
		return (put_error(man, 0, 0));
	return (1);
}

#ifdef __APPLE__
/* These window hints are to be called before creating the window */
static int	init_graphics_lib(void)
{
	if (!glfwInit())
		return (put_error(0, E_FAIL_GLFW, 0));
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	return (1);
}
#else
static int	init_graphics_lib(void)
{
	if (!glfwInit())
		return (put_error(0, E_FAIL_GLFW, 0));
	return (1);
}
#endif

static void	more_window_settings(t_man *man)
{
	t_ivec2	min;
	t_ivec2	max;

	set_ivec2(&min, man->res.window_size.x, man->res.window_size.y);
	set_ivec2(&max, man->res.monitor_size.x, man->res.monitor_size.y);
	glfwSetWindowSizeLimits(man->window, min.x, min.y, max.x, max.y);
	glfwMakeContextCurrent(man->window);
	set_initial_viewport(man);
	glfwSetFramebufferSizeCallback(man->window, framebuffer_size_callback);
	glfwSetWindowPosCallback(man->window, window_pos_callback);
	return ;
}

static void	set_initial_viewport(t_man *man)
{
	t_ivec2	size;

	glfwGetFramebufferSize(man->window, &size.x, &size.y);
	set_viewport(man, size);
	glViewport(man->res.viewport_offset.x, man->res.viewport_offset.y,
		man->res.viewport_size.x, man->res.viewport_size.y);
	return ;
}
