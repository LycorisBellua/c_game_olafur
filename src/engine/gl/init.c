#include "cimmerian.h"

#define ERR_WINDOW "Error: The window failed to create (the issue may be " \
	"with the GLFW library, but comes more likely from the OpenGL library / " \
	"check that your drivers are properly installed)"

void			framebuffer_size_callback(GLFWwindow *window, int x, int y);
void			window_pos_callback(GLFWwindow *window, int xpos, int ypos);

static int		init_graphics_lib(void);
static t_ivec2	get_monitor_size(void);
static void		more_window_settings(t_man *man);
static void		set_initial_viewport(t_man *man);

int	create_window(t_man *man, const char *title, int width, int height)
{
	if (!init_graphics_lib())
		return (0);
	set_resolution(man, get_monitor_size(), width, height);
	man->window = glfwCreateWindow(man->res.window_size.x,
		man->res.window_size.y, title, NULL, NULL);
	if (!man->window)
	{
		dprintf(STDERR_FILENO, ERR_WINDOW"\n");
		glfwTerminate();
		return (0);
	}
	more_window_settings(man);
	if (!init_gl_functions())
	{
		dprintf(STDERR_FILENO, "Error: Failed to load OpenGL functions\n");
		glfwTerminate();
		return (0);
	}
	man->shader_program = create_shader_program();
	if (!man->shader_program || !create_uniform(man) || !create_mesh())
	{
		deinit(man);
		return (0);
	}
	return (1);
}

static int	init_graphics_lib(void)
{
	if (!glfwInit())
	{
		dprintf(STDERR_FILENO, "Error: The GLFW library failed to "
			"initialize\n");
		return (0);
	}
	#ifdef __APPLE__
	/* These window hints are to be called before creating the window */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#endif
	return (1);
}

static t_ivec2	get_monitor_size(void)
{
	t_ivec2				monitor_size;
	const GLFWvidmode	*vid_mode;

	set_ivec2(&monitor_size, 0, 0);
	vid_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	if (vid_mode)
		set_ivec2(&monitor_size, vid_mode->width, vid_mode->height);
	return (monitor_size);
}

static void	more_window_settings(t_man *man)
{
	glfwSetWindowSizeLimits(man->window,
		/* min */ man->res.window_size.x, man->res.window_size.y,
		/* max */ man->res.monitor_size.x, man->res.monitor_size.y);
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
