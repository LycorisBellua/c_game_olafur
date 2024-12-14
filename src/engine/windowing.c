#include "cimmerian.h"

static void	set_res(const GLFWvidmode *vid_mode);
static void	set_viewport(int size_x, int size_y);
static void	set_initial_viewport(GLFWwindow *window);
static void	framebuffer_size_callback(GLFWwindow *window, int x, int y);
static void	window_pos_callback(GLFWwindow *window, int xpos, int ypos);

GLFWwindow	*get_window(const char *title)
{
	GLFWwindow	*window;

	if (!glfwInit())
	{
		dprintf(STDERR_FILENO, "Error: The GLFW library failed to "
			"initialize\n");
		exit(EXIT_FAILURE);
	}

	set_res(glfwGetVideoMode(glfwGetPrimaryMonitor()));

	#ifdef __APPLE__
	/* These window hints are to be called before creating the window */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#endif

	window = glfwCreateWindow(g_man.res.window_size.x, g_man.res.window_size.y,
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
		/* min */ g_man.res.window_size.x, g_man.res.window_size.y,
		/* max */ g_man.res.monitor_size.x, g_man.res.monitor_size.y);
	glfwMakeContextCurrent(window);
	set_initial_viewport(window);
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

void	toggle_fullscreen(GLFWwindow *window)
{
	int					decorated;
	GLFWmonitor			*monitor;
	const GLFWvidmode	*mode;

	decorated = !glfwGetWindowAttrib(window, GLFW_DECORATED);
	if (decorated)
	{
		/* Switch to windowed mode */
		glfwSetWindowMonitor(window, NULL,
			g_man.res.window_position.x, g_man.res.window_position.y,
			g_man.res.window_size.x, g_man.res.window_size.y, GLFW_DONT_CARE);
	}
	else
	{
		/* Switch to fullscreen */
		monitor = glfwGetPrimaryMonitor();
		mode = glfwGetVideoMode(monitor);
		glfwSetWindowMonitor(window, monitor, g_man.res.fullscreen.x,
			g_man.res.fullscreen.y, g_man.res.monitor_size.x,
			g_man.res.monitor_size.y, mode->refreshRate);
	}
	glfwSetWindowAttrib(window, GLFW_DECORATED, decorated);
	return ;
}

static void	set_res(const GLFWvidmode *vid_mode)
{
	g_man.res.monitor_size.x = vid_mode->width;
	g_man.res.monitor_size.y = vid_mode->height;
	g_man.res.aspect_ratio = (double)g_man.res.monitor_size.x /
		g_man.res.monitor_size.y;

	/* For monitors that are horizontally very long */
	if (g_man.res.aspect_ratio > 16.0 / 9)
	{
		g_man.res.aspect_ratio = 16.0 / 9;
		g_man.res.monitor_size.x = g_man.res.monitor_size.y *
			g_man.res.aspect_ratio;
	}

	g_man.res.window_size_default.x = min(640, g_man.res.monitor_size.x);
	g_man.res.window_size_default.y = min(640 / g_man.res.aspect_ratio,
			g_man.res.monitor_size.y);
	g_man.res.window_size.x = g_man.res.window_size_default.x;
	g_man.res.window_size.y = g_man.res.window_size_default.y;

	g_man.res.window_position.x = 0;
	g_man.res.window_position.y = 0;
	g_man.res.fullscreen.x = (vid_mode->width - g_man.res.monitor_size.x) / 2;
	g_man.res.fullscreen.y = (vid_mode->height - g_man.res.monitor_size.y) / 2;

	/* Height modifier for raycasting rendering */
	g_man.res.h_mod = g_man.res.aspect_ratio
		- (f_round(g_man.res.aspect_ratio) - g_man.res.aspect_ratio);
	return ;
}

static void	set_viewport(int size_x, int size_y)
{
	t_ivec2	new_size;
	t_ivec2	offset;

	new_size.x = size_x;
	new_size.y = size_x / g_man.res.aspect_ratio;
	if (new_size.y > size_y)
	{
		new_size.y = size_y;
		new_size.x = size_y * g_man.res.aspect_ratio;
	}
	offset.x = (size_x - new_size.x) / 2;
	offset.y = (size_y - new_size.y) / 2;
	glViewport(offset.x, offset.y, new_size.x, new_size.y);
	return ;
}

static void	set_initial_viewport(GLFWwindow *window)
{
	t_ivec2	size;

	glfwGetFramebufferSize(window, &size.x, &size.y);
	set_viewport(size.x, size.y);
	return ;
}

static void	framebuffer_size_callback(GLFWwindow *window, int x, int y)
{
	(void)window;
	set_viewport(x, y);

	/* for toggle_fullscreen() */
	if (glfwGetWindowAttrib(window, GLFW_DECORATED))
	{
		g_man.res.window_size.x = x;
		g_man.res.window_size.y = y;
	}
	return ;
}

static void	window_pos_callback(GLFWwindow *window, int xpos, int ypos)
{
	/* for toggle_fullscreen() */
	if (glfwGetWindowAttrib(window, GLFW_DECORATED))
	{
		g_man.res.window_position.x = xpos;
		g_man.res.window_position.y = ypos;
	}
	return ;
}
