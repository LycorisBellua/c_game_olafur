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
#include "../headers/shader_handling.h"
#include "../headers/uniforms.h"
#include "../headers/meshes.h"
#include "../headers/textures.h"
#include "../headers/colors.h"
#include "../headers/interfaces.h"
#include "../headers/camera.h"

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
    /* glfwSetCharCallback(window, char_key_callback); */

    glfwSwapInterval(1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (!create_shader_programs())
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    else
    {
        create_uniforms();
        create_meshes();
        create_textures();
        create_color_palette();
        initialize_interfaces();

        use_shader_program(shader_program_default);
        use_texture(TEX_MAIN);
    }

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        clear_drawing(TEX_MAIN, 1);
        active_interface->draw();
        save_drawing(TEX_MAIN);
        render_mesh(MESH_VIEWPORT, GL_TRIANGLES);

        move_camera();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    free_shader_programs();
    free_uniforms();
    free_meshes();
    free_textures();
    free_color_palette();

    list_arguments(argc, argv);
    return EXIT_SUCCESS;
}

