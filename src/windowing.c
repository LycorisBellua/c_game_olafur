#include "cimmerian.h"

static void set_resolution(const GLFWvidmode* vid_mode);
static void set_viewport(const int width, const int height);
static void set_initial_viewport(GLFWwindow* window);
static void framebuffer_size_callback(GLFWwindow* window, int width, 
    int height);
static void window_pos_callback(GLFWwindow* window, int xpos, int ypos);

GLFWwindow* get_window(const char* title)
{
    GLFWwindow* window;

    if (!glfwInit())
    {
        fprintf(stderr, "Error: The GLFW library failed to initialize\n");
        exit(EXIT_FAILURE);
    }

    set_resolution(glfwGetVideoMode(glfwGetPrimaryMonitor()));

    #ifdef __APPLE__
    /* These window hints are to be called before creating the window */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #endif

    window = glfwCreateWindow(man.res.window_width, man.res.window_height,
        title, NULL, NULL);
    if (!window)
    {
        fprintf(stderr, "Error: The window failed to create (the issue may "
            "be with the GLFW library, but comes more likely from the OpenGL "
            "library / check that your drivers are properly installed)\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetWindowSizeLimits(window, 
        /* min */ man.res.window_width, man.res.window_height, 
        /* max */ man.res.monitor_width, man.res.monitor_height);
    glfwMakeContextCurrent(window);
    set_initial_viewport(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetWindowPosCallback(window, window_pos_callback);

	if (!init_gl_functions())
	{
        fprintf(stderr, "Error: Failed to load OpenGL functions\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
	}

    return window;
}

void toggle_fullscreen(GLFWwindow* window)
{
    const int decorated = !glfwGetWindowAttrib(window, GLFW_DECORATED);
    GLFWmonitor* monitor;
    const GLFWvidmode* mode;

    if (decorated)
    {
        /* Switch to windowed mode */
        glfwSetWindowMonitor(window, NULL, 
            man.res.window_position_x, man.res.window_position_y, 
            man.res.window_width, man.res.window_height, GLFW_DONT_CARE);
    }
    else
    {
        /* Switch to fullscreen */
        monitor = glfwGetPrimaryMonitor();
        mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(window, monitor, man.res.fullscreen_x, 
            man.res.fullscreen_y, man.res.monitor_width, man.res.monitor_height, 
            mode->refreshRate);
    }

    glfwSetWindowAttrib(window, GLFW_DECORATED, decorated);
    return;
}

static void set_resolution(const GLFWvidmode* vid_mode)
{
    man.res.monitor_width = vid_mode->width;
    man.res.monitor_height = vid_mode->height;
    man.res.aspect_ratio = (double)man.res.monitor_width / man.res.monitor_height;

    /* For monitors that are horizontally very long */
    if (man.res.aspect_ratio > 16.0/9)
    {
        man.res.aspect_ratio = 16.0/9;
        man.res.monitor_width = man.res.monitor_height * man.res.aspect_ratio;
    }

    man.res.window_width_default = min(640, man.res.monitor_width);
    man.res.window_height_default = min(640/man.res.aspect_ratio, man.res.monitor_height);
    man.res.window_width = man.res.window_width_default;
    man.res.window_height = man.res.window_height_default;

    man.res.window_position_x = 0;
    man.res.window_position_y = 0;
    man.res.fullscreen_x = (vid_mode->width - man.res.monitor_width) / 2;
    man.res.fullscreen_y = (vid_mode->height - man.res.monitor_height) / 2;
    return;
}

static void set_viewport(const int width, const int height)
{
    int new_width = width;
    int new_height = width / man.res.aspect_ratio;
    int x_offset, y_offset;

    if (new_height > height)
    {
        new_height = height;
        new_width = height * man.res.aspect_ratio;
    }

    x_offset = (width - new_width) / 2;
    y_offset = (height - new_height) / 2;

    glViewport(x_offset, y_offset, new_width, new_height);
    return;
}

static void set_initial_viewport(GLFWwindow* window)
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    set_viewport(width, height);
    return;
}

static void framebuffer_size_callback
(
    __attribute__((unused))GLFWwindow* window, 
    int width, 
    int height
)
{
    set_viewport(width, height);

    /* for toggle_fullscreen() */
    if (glfwGetWindowAttrib(window, GLFW_DECORATED))
    {
        man.res.window_width = width;
        man.res.window_height = height;
    }
    return;
}

static void window_pos_callback(GLFWwindow* window, int xpos, int ypos)
{
    /* for toggle_fullscreen() */
    if (glfwGetWindowAttrib(window, GLFW_DECORATED))
    {
        man.res.window_position_x = xpos;
        man.res.window_position_y = ypos;
    }
    return;
}
