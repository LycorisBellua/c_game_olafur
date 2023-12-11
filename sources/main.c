#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>
#include "../headers/windowing.h"
#include "../headers/input.h"
#include "../headers/immediate_rendering_mode.h"

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

    /* Input handling */
    glfwSetKeyCallback(window, physical_key_callback);
    /* glfwSetCharCallback(window, char_key_callback); */

    /*
        Buffer swapping
        -----------------------------------------------------------------------
        OpenGL is a double buffering lib, meaning there's a front buffer and a 
        back buffer: the front buffer is what's displayed on screen, while the 
        back buffer is what's currently being rendered. Once the frame has 
        finished rendering in the back buffer, it's ready to be displayed, 
        which means that the back and the front buffers will swap. Without 
        double buffering, the frame would be drawn right before our eyes and 
        it wouldn't look good.

        The line below sets the swap interval, which is 0 by default, it 
        should be 1 instead. 0 means to swap as soon as the frame is done 
        rendering, and on a capable machine the rendering will be quite fast, 
        probably even faster than the screen's refresh rate. Having more FPS 
        than the screen can handle means that those extra frames won't be 
        displayed: the library technically displays them, but the screen can't 
        follow, so they're not actually displayed. They're rendered for 
        nothing, and this rendering still costs resources, resources that 
        can't be used for other programs for example. This is not very nice to 
        the user, so we set the swap interval to 1. This value is the sweet 
        spot, because above it may cause input latency.
    */
    glfwSwapInterval(1);

    /*
        Depth-testing
        -----------------------------------------------------------------------
        Compare the depth value of shapes at a given position. Whichever has 
        the smallest depth is considered closer to the viewer, and the other 
        shapes are considered hidden by it. Only draw the pixels of the shapes 
        that should be visible on screen, and discard the rest.
    */
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    /* For shaders */
    printf("GLSL version: %d\n", glsl_version);

    while (!glfwWindowShouldClose(window))
    {
        /*
            Buffer clearing
            -------------------------------------------------------------------
            Buffer clearing is the first step of the rendering process. Here 
            we are not refering to the front and back buffers, but to other 
            buffers: color, depth, stencil and accumulation buffers. The back 
            buffer uses these buffers to render the frame, so if they're not 
            cleared then using both old and new data may lead to artifacts. Of 
            course, there may not be a huge difference between the current 
            frame and the next one, so instead of drawing the entire frame we 
            could implement smart rendering: modify the part of the buffers 
            which should be different, and leave the rest as is. For the sake 
            of simplicity, we'll just clear the buffers.

            In 2D graphics, we can get away with only clearing the color 
            buffer, but the depth buffer needs to be cleared as well in 3D 
            graphics or we'll get artifacts. As for stencil and accumulation, 
            they are used in complex effects, so for our use case we only care 
            about clearing the color and depth buffers.
        */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /*
        irm_triangle_white();
        */
        irm_triangle_rgb();
        irm_viewport_white();

        /* Display frame */
        glfwSwapBuffers(window);
        /* Read input events */
        glfwPollEvents();
    }
    glfwTerminate();

    list_arguments(argc, argv);
    return EXIT_SUCCESS;
}

