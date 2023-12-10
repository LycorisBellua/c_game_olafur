#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../headers/windowing.h"

#define MIN(a, b) (a < b ? a : b)
#define WINDOW_DEFAULT_WIDTH 640
#define WINDOW_DEFAULT_HEIGHT 480

GLFWwindow* get_window(const char* title)
{
    GLFWwindow* window;
    const GLFWvidmode* vid_mode;
    int window_size[2];

    if (!glfwInit())
    {
        fprintf(stderr, "ERROR: The GLFW library failed to initialize.\n");
        exit(EXIT_FAILURE);
    }

    vid_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    window_size[0] = MIN(WINDOW_DEFAULT_WIDTH, vid_mode->width);
    window_size[1] = MIN(WINDOW_DEFAULT_HEIGHT, vid_mode->height);

    #ifdef __APPLE__
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

    glfwMakeContextCurrent(window);
    glfwSetWindowSizeLimits(window, 
        /* min */ window_size[0], window_size[1], 
        /* max */ vid_mode->width, vid_mode->height);

    /* GLEW includes the latest version of OpenGL available on the machine */
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) /* GLEW_OK is zero */
    {
        fprintf(stderr, "ERROR: The GLEW library failed to initialize.\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    /*
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("OpenGL version supported: %s\n", glGetString(GL_VERSION));
    */
    return window;
}

