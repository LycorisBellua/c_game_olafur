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
#include "../headers/rendering.h"

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

    const char* vs_filepath = "shaders/vs.glsl";
    const char* fs_filepath = "shaders/fs.glsl";
    GLuint vs = 0;
    GLuint fs = 0;
    GLuint shader_program = 0;

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

    set_app_glsl_version();
    vs = compile_shader(GL_VERTEX_SHADER, vs_filepath);
    fs = compile_shader(GL_FRAGMENT_SHADER, fs_filepath);
    shader_program = create_shader_program(window, vs, fs);
    /*
        The shaders are already compiled in the shader program, so no need to 
        keep them around unless you want to use them in another shader program.
    */
    free_shader(&vs);
    free_shader(&fs);

    create_uniforms(shader_program);
    create_meshes();
    initialize_interfaces();

    /* Switch to another shader program by calling this function again */
    if (shader_program)
        glUseProgram(shader_program);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        active_interface->render();
        move_player();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    free_shader_program(&shader_program);
    free_uniforms();
    free_meshes();
    glfwTerminate();

    list_arguments(argc, argv);
    return EXIT_SUCCESS;
}

