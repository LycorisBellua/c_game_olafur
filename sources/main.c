#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../headers/windowing.h"
#include "../headers/input.h"
#include "../headers/rendering.h"

float points[] =
{
    0.0f,  0.5f,  0.0f,
    0.5f, -0.5f,  0.0f,
    -0.5f, -0.5f,  0.0f
};

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

    int glsl_version = get_glsl_version();
    GLuint vao, vs, fs, shader_program;
    const char* vs_filepath = "shaders/vs_position.glsl";
    const char* fs_filepath = "shaders/fs_hardcoded_unicolor.glsl";

    glfwSetKeyCallback(window, physical_key_callback);
    /* glfwSetCharCallback(window, char_key_callback); */

    glfwSwapInterval(1);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    /* TODO: */
    printf("GLSL version: %d\n", glsl_version);

    vao = create_mesh_vao(points, sizeof(points)/sizeof(float), 3, 
        GL_STATIC_DRAW);

    vs = compile_shader(GL_VERTEX_SHADER, vs_filepath);
    fs = compile_shader(GL_FRAGMENT_SHADER, fs_filepath);
    shader_program = create_shader_program(vs, fs);
    if (!shader_program)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        fprintf(stderr, "ERROR: Couldn't compile shader program from \"%s\" "
            "vertex shader and \"%s\" fragment shader.\n", vs_filepath, 
            fs_filepath);
    }
    /*
        The shaders are already compiled in the shader program, so no need to 
        keep them around unless you want to use them in another shader program.
    */
    free_shader(vs);
    free_shader(fs);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        render_mesh(shader_program, vao, GL_TRIANGLES, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    free_mesh(vao);
    free_shader_program(shader_program);
    glfwTerminate();

    list_arguments(argc, argv);
    return EXIT_SUCCESS;
}

