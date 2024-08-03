#include "../hdr/windowing.h"
#include "../hdr/maths.h"
#include "../hdr/gl_functions.h"

Resolution res = {0};

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

    window = glfwCreateWindow(res.window_width, res.window_height, title, 
        NULL, NULL);
    if (!window)
    {
        fprintf(stderr, "Error: The window failed to create (the issue may "
            "be with the GLFW library, but comes more likely from the OpenGL "
            "library / check that your drivers are properly installed)\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetWindowSizeLimits(window, 
        /* min */ res.window_width, res.window_height, 
        /* max */ res.monitor_width, res.monitor_height);
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
            res.window_position_x, res.window_position_y, 
            res.window_width, res.window_height, GLFW_DONT_CARE);
    }
    else
    {
        /* Switch to fullscreen */
        monitor = glfwGetPrimaryMonitor();
        mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(window, monitor, res.fullscreen_x, 
            res.fullscreen_y, res.monitor_width, res.monitor_height, 
            mode->refreshRate);
    }

    glfwSetWindowAttrib(window, GLFW_DECORATED, decorated);
    return;
}

static void set_resolution(const GLFWvidmode* vid_mode)
{
    res.monitor_width = vid_mode->width;
    res.monitor_height = vid_mode->height;
    res.aspect_ratio = (float)res.monitor_width / res.monitor_height;

    /* For monitors that are horizontally very long */
    if (res.aspect_ratio > 16.0/9)
    {
        res.aspect_ratio = 16.0/9;
        res.monitor_width = res.monitor_height * res.aspect_ratio;
    }

    res.window_width_default = MIN(640, res.monitor_width);
    res.window_height_default = MIN(640/res.aspect_ratio, res.monitor_height);
    res.window_width = res.window_width_default;
    res.window_height = res.window_height_default;

    res.window_position_x = 0;
    res.window_position_y = 0;
    res.fullscreen_x = (vid_mode->width - res.monitor_width) / 2;
    res.fullscreen_y = (vid_mode->height - res.monitor_height) / 2;
    return;
}

static void set_viewport(const int width, const int height)
{
    int new_width = width;
    int new_height = width / res.aspect_ratio;
    int x_offset, y_offset;

    if (new_height > height)
    {
        new_height = height;
        new_width = height * res.aspect_ratio;
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
        res.window_width = width;
        res.window_height = height;
    }
    return;
}

static void window_pos_callback(GLFWwindow* window, int xpos, int ypos)
{
    /* for toggle_fullscreen() */
    if (glfwGetWindowAttrib(window, GLFW_DECORATED))
    {
        res.window_position_x = xpos;
        res.window_position_y = ypos;
    }
    return;
}

