#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>
#include "../headers/windowing.h"
#include "../headers/input.h"

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

int main(int argc, char** argv)
{
    const char* title = "Cimmerian";
    GLFWwindow* window = get_window(title);
    glfwSetKeyCallback(window, physical_key_callback);
    /* TODO: The char callback may not be useful anymore */
    glfwSetCharCallback(window, char_key_callback);

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

