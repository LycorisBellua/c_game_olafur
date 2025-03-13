#include "cimmerian.h"

static void	set_res(t_man *man, const GLFWvidmode *vid_mode);
static void	set_viewport(t_man *man, t_ivec2 framebuffer_size);
static void	set_initial_viewport(t_man *man, GLFWwindow *window);
static void	framebuffer_size_callback(GLFWwindow *window, int x, int y);
static void	window_pos_callback(GLFWwindow *window, int xpos, int ypos);

GLFWwindow	*get_window(t_man *man, const char *title)
{
	GLFWwindow	*window;

	if (!glfwInit())
	{
		dprintf(STDERR_FILENO, "Error: The GLFW library failed to "
			"initialize\n");
		exit(EXIT_FAILURE);
	}

	set_res(man, glfwGetVideoMode(glfwGetPrimaryMonitor()));

	#ifdef __APPLE__
	/* These window hints are to be called before creating the window */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#endif

	window = glfwCreateWindow(man->res.window_size.x, man->res.window_size.y,
			title, NULL, NULL);
	if (!window)
	{
		dprintf(STDERR_FILENO, "Error: The window failed to create (the issue "
			"may be with the GLFW library, but comes more likely from the "
			"OpenGL library / check that your drivers are properly "
			"installed)\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetWindowSizeLimits(window,
		/* min */ man->res.window_size.x, man->res.window_size.y,
		/* max */ man->res.monitor_size.x, man->res.monitor_size.y);
	glfwMakeContextCurrent(window);
	set_initial_viewport(man, window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetWindowPosCallback(window, window_pos_callback);

	if (!init_gl_functions())
	{
		dprintf(STDERR_FILENO, "Error: Failed to load OpenGL functions\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	return (window);
}

void	toggle_fullscreen(t_man *man, GLFWwindow *window)
{
	int					decorated;
	GLFWmonitor			*monitor;
	const GLFWvidmode	*mode;

	decorated = !glfwGetWindowAttrib(window, GLFW_DECORATED);
	if (decorated)
	{
		/* Switch to windowed mode */
		glfwSetWindowMonitor(window, NULL,
			man->res.window_position.x, man->res.window_position.y,
			man->res.window_size.x, man->res.window_size.y, GLFW_DONT_CARE);
	}
	else
	{
		/* Switch to fullscreen */
		monitor = glfwGetPrimaryMonitor();
		mode = glfwGetVideoMode(monitor);
		glfwSetWindowMonitor(window, monitor, man->res.fullscreen.x,
			man->res.fullscreen.y, man->res.monitor_size.x,
			man->res.monitor_size.y, mode->refreshRate);
	}
	glfwSetWindowAttrib(window, GLFW_DECORATED, decorated);
	return ;
}

static void	set_res(t_man *man, const GLFWvidmode *vid_mode)
{
	man->res.monitor_size.x = vid_mode->width;
	man->res.monitor_size.y = vid_mode->height;
	man->res.aspect_ratio = (double)man->res.monitor_size.x /
		man->res.monitor_size.y;

	/* For monitors that are horizontally very long */
	if (man->res.aspect_ratio > 16.0 / 9)
	{
		man->res.aspect_ratio = 16.0 / 9;
		man->res.monitor_size.x = man->res.monitor_size.y *
			man->res.aspect_ratio;
	}

	man->res.window_size_default.x = min(RES_WIDTH, man->res.monitor_size.x);
	man->res.window_size_default.y = min(RES_WIDTH / man->res.aspect_ratio,
			man->res.monitor_size.y);
	man->res.window_size.x = man->res.window_size_default.x;
	man->res.window_size.y = man->res.window_size_default.y;

	man->res.window_position.x = 0;
	man->res.window_position.y = 0;
	man->res.fullscreen.x = (vid_mode->width - man->res.monitor_size.x) / 2;
	man->res.fullscreen.y = (vid_mode->height - man->res.monitor_size.y) / 2;

	/* Height modifier for raycasting rendering */
	man->res.h_mod = man->res.aspect_ratio
		- (round_f(man->res.aspect_ratio) - man->res.aspect_ratio);
	return ;
}

static void	set_viewport(t_man *man, t_ivec2 framebuffer_size)
{
	man->res.viewport_size.x = framebuffer_size.x;
	man->res.viewport_size.y = framebuffer_size.x / man->res.aspect_ratio;
	if (man->res.viewport_size.y > framebuffer_size.y)
	{
		man->res.viewport_size.y = framebuffer_size.y;
		man->res.viewport_size.x = framebuffer_size.y * man->res.aspect_ratio;
	}
	man->res.viewport_offset.x = (framebuffer_size.x
			- man->res.viewport_size.x) / 2;
	man->res.viewport_offset.y = (framebuffer_size.y
			- man->res.viewport_size.y) / 2;
	glViewport(man->res.viewport_offset.x, man->res.viewport_offset.y,
		man->res.viewport_size.x, man->res.viewport_size.y);
	return ;
}

static void	set_initial_viewport(t_man *man, GLFWwindow *window)
{
	t_ivec2	size;

	glfwGetFramebufferSize(window, &size.x, &size.y);
	set_viewport(man, size);
	return ;
}

static void	framebuffer_size_callback(GLFWwindow *window, int x, int y)
{
	t_ivec2	size;

	(void)window;
	set_ivec2(&size, x, y);
	set_viewport(&g_man, size);

	/* for toggle_fullscreen() */
	if (glfwGetWindowAttrib(window, GLFW_DECORATED))
		set_ivec2(&g_man.res.window_size, x, y);
	return ;
}

static void	window_pos_callback(GLFWwindow *window, int xpos, int ypos)
{
	/* for toggle_fullscreen() */
	if (glfwGetWindowAttrib(window, GLFW_DECORATED))
		set_ivec2(&g_man.res.window_position, xpos, ypos);
	return ;
}
