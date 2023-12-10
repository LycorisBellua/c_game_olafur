#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>

#define MIN(a, b) (a < b ? a : b)
#define WINDOW_DEFAULT_WIDTH 640
#define WINDOW_DEFAULT_HEIGHT 480

void list_arguments(int argc, char** argv)
{
    int i;
    printf("Debug: %d arguments\n", argc - 1);
    if (argc > 1)
    {
        for (i = 1; i < argc; ++i)
            printf("%d. %s\n", i, argv[i]);
    }
    return;
}

void physical_key_callback
(
    GLFWwindow* window, 
    int key, 
    __attribute__((unused))int scancode, 
    int action, 
    __attribute__((unused))int mods
)
{
    switch (key)
    {
        case GLFW_KEY_ESCAPE:
            /* 
               By default, the window closing event is triggered by the cross 
               window button or Alt+F4. We also decide to set the Escape key 
               as such.

               Any of these closing inputs will set 
               `glfwWindowShouldClose(window)` to GLFW_TRUE, after which the 
               window can be closed with `glfwDestroyWindow(window)`, or the 
               whole program (windows included) can be closed with 
               `glfwTerminate()`.
            */
            if (action == GLFW_PRESS)
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;

        case GLFW_KEY_UP:
        case GLFW_KEY_W:
            if (action == GLFW_PRESS)
                printf("Debug: UP started\n");
            else if (action == GLFW_REPEAT)
                printf("Debug: UP performed\n");
            else
                printf("Debug: UP canceled\n");
            break;

        case GLFW_KEY_DOWN:
        case GLFW_KEY_S:
            if (action == GLFW_PRESS)
                printf("Debug: DOWN started\n");
            else if (action == GLFW_REPEAT)
                printf("Debug: DOWN performed\n");
            else
                printf("Debug: DOWN canceled\n");
            break;

        case GLFW_KEY_RIGHT:
        case GLFW_KEY_D:
            if (action == GLFW_PRESS)
                printf("Debug: RIGHT started\n");
            else if (action == GLFW_REPEAT)
                printf("Debug: RIGHT performed\n");
            else
                printf("Debug: RIGHT canceled\n");
            break;

        case GLFW_KEY_LEFT:
        case GLFW_KEY_A:
            if (action == GLFW_PRESS)
                printf("Debug: LEFT started\n");
            else if (action == GLFW_REPEAT)
                printf("Debug: LEFT performed\n");
            else
                printf("Debug: LEFT canceled\n");
            break;

        case GLFW_KEY_Q:
            if (action == GLFW_PRESS)
                printf("Debug: STRAFE LEFT started\n");
            else if (action == GLFW_REPEAT)
                printf("Debug: STRAFE LEFT performed\n");
            else
                printf("Debug: STRAFE LEFT canceled\n");
            break;

        case GLFW_KEY_E:
            if (action == GLFW_PRESS)
                printf("Debug: STRAFE RIGHT started\n");
            else if (action == GLFW_REPEAT)
                printf("Debug: STRAFE RIGHT performed\n");
            else
                printf("Debug: STRAFE RIGHT canceled\n");
            break;
    }
    return;
}

void char_key_callback
(
    __attribute__((unused))GLFWwindow* window, 
    unsigned int codepoint
)
{
    /* TODO: Print with UTF-8 encoding */
    printf("Debug (char callback): %c\n", codepoint);
    return;
}

int main(int argc, char** argv)
{
    GLFWwindow* window;
    const GLFWvidmode* vid_mode;
    int window_size[2];
    const char* title = "Cimmerian";

    if (!glfwInit())
    {
        fprintf(stderr, "ERROR: The GLFW library failed to initialize.\n");
        return EXIT_FAILURE;
    }

    vid_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    window_size[0] = MIN(WINDOW_DEFAULT_WIDTH, vid_mode->width);
    window_size[1] = MIN(WINDOW_DEFAULT_HEIGHT, vid_mode->height);
    window = glfwCreateWindow(window_size[0], window_size[1], title, NULL, 
        NULL);
    if (!window)
    {
        fprintf(stderr, "ERROR: The window failed to create. The issue may "
            "be with the GLFW library, but comes more likely from the OpenGL "
            "library. Check that your drivers are properly installed.\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwSetWindowSizeLimits(window, 
        /* min */ window_size[0], window_size[1], 
        /* max */ vid_mode->width, vid_mode->height);

    glfwSetKeyCallback(window, physical_key_callback);
    glfwSetCharCallback(window, char_key_callback);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window))
    {
        /* OpenGL here */
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();

    list_arguments(argc, argv);
    return EXIT_SUCCESS;
}

