#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../headers/windowing.h"
#include "../headers/input.h"
#include "../headers/shader_handling.h"
#include "../headers/uniforms.h"
#include "../headers/meshes.h"
#include "../headers/interfaces.h"
#include "../headers/transform.h"

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
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    /*
        Line width should be 1.0f by default, but it's better to set it 
        explicitely to know which width we deal with. Set it to 1.5f otherwise 
        lines at -1.0 on the X or Y axis won't show in the viewport. It's fine 
        with a filled-up usage such as GL_TRIANGLES, but not when it comes to 
        rendering lines. The visual difference between 1.0 and 1.5 is minimal 
        enough.
    */
    glLineWidth(1.5f);

    if (!create_shader_programs())
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    else
    {
        create_uniforms();
        create_meshes();
        initialize_interfaces();

        shader_program_world->use(shader_program_world);
        /* shader_program_ui->use(shader_program_ui); */
    }

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        active_interface->render();
        move_player();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    free_shader_programs();
    free_uniforms();
    free_meshes();
    glfwTerminate();

    list_arguments(argc, argv);
    return EXIT_SUCCESS;
}

