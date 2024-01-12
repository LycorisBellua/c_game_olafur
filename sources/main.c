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

GLuint create_texture(void)
{
    GLuint id_texture = 0;
    int texture_width = 256;
    int texture_height = 256;
    int x, y, index;

    /* Create a buffer to store texture data (RGBA format) */
    unsigned char* texture_data = malloc(texture_width * texture_height * 4 * 
        sizeof(unsigned char));

    if (!texture_data)
        return -1;

    glGenTextures(1, &id_texture);
    glBindTexture(GL_TEXTURE_2D, id_texture);

    /* Set texture parameters (you can customize these) */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /*
       Fill the buffer with your texture data (for example, draw lines and 
       shapes). Here, we fill the entire texture with a gradient.
    */
    for (y = 0; y < texture_height; ++y)
    {
        for (x = 0; x < texture_width; ++x)
        {
            index = (y * texture_width + x) * 4;
            texture_data[index]   = (unsigned char)(x * 255 / texture_width);
            texture_data[index+1] = (unsigned char)(y * 255 / texture_height);
            texture_data[index+2] = 0;
            texture_data[index+3] = 255;  /* Alpha channel */
        }
    }

    /* Set the texture data */
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height, 0, 
        GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

    /* Unbind the texture */
    glBindTexture(GL_TEXTURE_2D, 0);

    return id_texture;
}

int main(int argc, char** argv)
{
    const char* title = "Cimmerian";
    GLFWwindow* window = get_window(title);

    glfwSetKeyCallback(window, physical_key_callback);
    /* glfwSetCharCallback(window, char_key_callback); */

    glfwSwapInterval(1);
    glEnable(GL_TEXTURE_2D);
    /*
        Line width should be 1.0f by default, but it's better to set it 
        explicitly to know which width we deal with. Set it to 1.5f otherwise 
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

        glBindTexture(GL_TEXTURE_2D, create_texture());
        use_shader_program(shader_program_default);
    }

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        active_interface->render();
        move_camera();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    free_shader_programs();
    free_uniforms();
    free_meshes();

    list_arguments(argc, argv);
    return EXIT_SUCCESS;
}

