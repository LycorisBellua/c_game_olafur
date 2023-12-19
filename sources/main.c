#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../headers/windowing.h"
#include "../headers/input.h"
#include "../headers/shader_handling.h"
#include "../headers/uniforms.h"
#include "../headers/meshes.h"
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
    UniformStruct* color_uniform = 0;

    GLuint mesh = 0;
    int nbr_vertices;
    GLuint mesh1 = 0;
    int nbr_vertices1;

    glfwSetKeyCallback(window, physical_key_callback);
    /* glfwSetCharCallback(window, char_key_callback); */

    glfwSwapInterval(1);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

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
    color_uniform = create_uniform(shader_program, "single_color", 
        activate_uniform_vec3, 0.4f, 0.21f, 0.5f);

    convert_vertex_positions_to_aspect_ratio(get_aspect_ratio());
    mesh = create_mesh(MESH_TRIANGLE, &nbr_vertices);
    mesh1 = create_mesh(MESH_SQUARE, &nbr_vertices1);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        color_uniform->activate(color_uniform, 1);
        render_mesh(shader_program, mesh1, GL_LINE_LOOP, nbr_vertices1);

        color_uniform->activate(color_uniform, 0);
        render_mesh(shader_program, mesh, GL_TRIANGLES, nbr_vertices);

        /*
            *((float*)color_uniform->data + 2) = 0.0f;
        */

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    free_shader_program(&shader_program);
    free_uniform(&color_uniform);
    free_mesh(&mesh);
    free_mesh(&mesh1);
    glfwTerminate();

    list_arguments(argc, argv);
    return EXIT_SUCCESS;
}

