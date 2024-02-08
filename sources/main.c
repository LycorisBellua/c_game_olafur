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

#include "../headers/command_line.h"
#include "../headers/windowing.h"
#include "../headers/input.h"
#include "../headers/shader_program.h"
#include "../headers/uniform.h"
#include "../headers/mesh.h"
#include "../headers/colors.h"
#include "../headers/textures.h"
#include "../headers/interfaces.h"
#include "../headers/time.h"
#include "../headers/maps.h"
#include "../headers/player.h"

int main(int argc, char** argv)
{
    const char* title = "Cimmerian";
    const GLubyte background_color = get_color_from_rgb(MAX_RED/3, MAX_GREEN/3, 
        MAX_BLUE/3); /* Dark grey */
    GLFWwindow* window = get_window(title);

    glfwSetKeyCallback(window, physical_key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    enable_vsync(1);

    if (!create_shader_program())
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    else
    {
        create_uniform();
        create_mesh();
        create_color_palette();
        create_textures();
        initialize_interfaces();
        initialize_maps();

        use_texture(TEX_MAIN);
        execute_cli_options(argc, argv);
    }

    while (!glfwWindowShouldClose(window))
    {
        update_time_variables();
        /*
        printf("FPS: %.2f\n", fps_count);
        */

        /* Rendering */
        glClear(GL_COLOR_BUFFER_BIT);
        clear_drawing(TEX_MAIN, background_color);
        active_interface->draw();
        save_drawing(TEX_MAIN);
        render_mesh();

        /* Input */
        update_player_transform(map_test);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    free_shader_program();
    free_uniform();
    free_mesh();
    free_textures();
    free_maps();
    return EXIT_SUCCESS;
}

