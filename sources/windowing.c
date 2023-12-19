#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../headers/windowing.h"

#define MIN(a, b) (a < b ? a : b)
#define WINDOW_DEFAULT_WIDTH 640
#define WINDOW_DEFAULT_HEIGHT 480

static int window_size[2] = {0};
static int window_pos[2] = {0};
static float aspect_ratio = 0;

static void set_aspect_ratio_viewport(int width, int height)
{
    int new_width = width;
    int new_height = width / aspect_ratio;
    int x_offset, y_offset;

    if (new_height > height)
    {
        new_height = height;
        new_width = height * aspect_ratio;
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
    set_aspect_ratio_viewport(width, height);
    return;
}

static void framebuffer_size_callback
(
    __attribute__((unused))GLFWwindow* window, 
    int width, 
    int height
)
{
    set_aspect_ratio_viewport(width, height);

    /* for toggle_fullscreen() */
    if (glfwGetWindowAttrib(window, GLFW_DECORATED))
    {
        window_size[0] = width;
        window_size[1] = height;
    }
    return;
}

void window_pos_callback(GLFWwindow* window, int xpos, int ypos)
{
    /* for toggle_fullscreen() */
    if (glfwGetWindowAttrib(window, GLFW_DECORATED))
    {
        window_pos[0] = xpos;
        window_pos[1] = ypos;
    }
    return;
}

GLFWwindow* get_window(const char* title)
{
    GLFWwindow* window;
    const GLFWvidmode* vid_mode;

    if (!glfwInit())
    {
        fprintf(stderr, "ERROR: The GLFW library failed to initialize.\n");
        exit(EXIT_FAILURE);
    }

    vid_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    window_size[0] = MIN(WINDOW_DEFAULT_WIDTH, vid_mode->width);
    window_size[1] = MIN(WINDOW_DEFAULT_HEIGHT, vid_mode->height);
    aspect_ratio = (float)vid_mode->width / vid_mode->height;

    #ifdef __APPLE__
    /* These window hints are to be called before creating the window */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #endif

    window = glfwCreateWindow(window_size[0], window_size[1], title, NULL, 
        NULL);
    if (!window)
    {
        fprintf(stderr, "ERROR: The window failed to create. The issue may "
            "be with the GLFW library, but comes more likely from the OpenGL "
            "library. Check that your drivers are properly installed.\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetWindowSizeLimits(window, 
        /* min */ window_size[0], window_size[1], 
        /* max */ vid_mode->width, vid_mode->height);
    glfwMakeContextCurrent(window);
    set_initial_viewport(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetWindowPosCallback(window, window_pos_callback);

    /*
        GLEW includes the latest version of OpenGL available on the machine. 
        Its header is to be included before GLFW's in a source file. 
        And `glewExperimental` is to be set before GLEW is initialized.
    */
    glewExperimental = GL_TRUE;
    /* GLEW_OK is zero */
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "ERROR: The GLEW library failed to initialize.\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    return window;
}

void toggle_fullscreen(GLFWwindow* window)
{
    int decorated = !glfwGetWindowAttrib(window, GLFW_DECORATED);
    GLFWmonitor* monitor;
    const GLFWvidmode* mode;

    if (decorated)
    {
        /* Switch to windowed mode */
        glfwSetWindowMonitor(window, NULL, 
            window_pos[0], window_pos[1], 
            window_size[0], window_size[1], GLFW_DONT_CARE);
    }
    else
    {
        /* Switch to fullscreen */
        monitor = glfwGetPrimaryMonitor();
        mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, 
            mode->refreshRate);
    }

    glfwSetWindowAttrib(window, GLFW_DECORATED, decorated);
    return;
}

