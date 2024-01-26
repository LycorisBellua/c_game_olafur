#include <stdio.h>
#include <stdlib.h>

#ifndef __GLEW_H__
#define __GLEW_H__
#include <GL/glew.h>
#endif

#ifndef __GLFW_H__
#define __GLFW_H__
#include <GLFW/glfw3.h>
#endif

#include "../headers/windowing.h"
#include "../headers/input.h"
#include "../headers/shader_program.h"
#include "../headers/uniform.h"
#include "../headers/mesh.h"
#include "../headers/colors.h"
#include "../headers/textures.h"
#include "../headers/interfaces.h"

static void list_arguments(int argc, char** argv)
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
    glfwSwapInterval(1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (!create_shader_program())
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    else
    {
        create_uniform();
        create_mesh();
        create_color_palette();
        create_textures();
        initialize_interfaces();

        use_texture(TEX_MAIN);
    }

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        clear_drawing(TEX_MAIN);
        active_interface->draw();
        save_drawing(TEX_MAIN);
        render_mesh();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    free_shader_program();
    free_uniform();
    free_mesh();
    free_textures();

    list_arguments(argc, argv);
    return EXIT_SUCCESS;
}

