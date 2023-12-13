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

const char* vs =
"#version 400\n"
"in vec3 vp;"
"void main()"
"{"
"  gl_Position = vec4(vp, 1.0);"
"}";

const char* fs =
"#version 400\n"
"out vec4 frag_colour;"
"void main()"
"{"
"  frag_colour = vec4(0.5, 0.8, 1.0, 1.0);"
"}";

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
    GLuint vao, shader_program;

    glfwSetKeyCallback(window, physical_key_callback);
    /* glfwSetCharCallback(window, char_key_callback); */

    glfwSwapInterval(1);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    printf("GLSL version: %d\n", glsl_version);
    printf("Todo: Load shaders from files\n");
    vao = create_mesh_vao(points, sizeof(points)/sizeof(float), 3, 
        GL_STATIC_DRAW);
    shader_program = create_shader_program(vs, fs);

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

