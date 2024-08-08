#include "cimmerian.h"

static GLFWwindow* init(const char* title);
static void draw(void);
static void deinit(void);

int main(void)
{
    const char* title = "Cimmerian";
    GLFWwindow* window = init(title);
    
    if (!window)
        return EXIT_FAILURE;

    glfwSetKeyCallback(window, physical_key_callback);
    glfwSetScrollCallback(window, scroll_callback);

    while (!glfwWindowShouldClose(window))
    {
        update_time_variables();
        /* printf("FPS: %.2f\n", fps_count); */

        clear_drawing(TEX_MAIN);
        draw();

        /* update_player_transform(map_test); */

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    deinit();
    return EXIT_SUCCESS;
}

static GLFWwindow* init(const char* title)
{
    GLFWwindow* window;

    window = get_window(title);
    if (!create_shader_program())
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        return 0;
    }
    else
    {
        create_uniform();
        create_mesh();
        create_textures();
        initialize_interfaces();
        initialize_maps();
        use_texture(TEX_MAIN);
    }
    return window;
}

static void draw(void)
{
    active_interface->draw();
    save_drawing(TEX_MAIN);
    render_mesh();
    return;
}

static void deinit(void)
{
    glfwTerminate();
    free_shader_program();
    free_uniform();
    free_mesh();
    free_textures();
    free_maps();
    return;
}
