#include "cimmerian.h"

t_manager man;

static GLFWwindow* init(char* title);
static void update_time_variables(void);
static void clear_buffer(void);
static void draw_into_buffer(void);
static void swap_buffer(GLFWwindow* window);
static void deinit(void);

int main(void)
{
    char* title = "Cimmerian";
    GLFWwindow* window = init(title);
    
    if (!window)
        return EXIT_FAILURE;

    glfwSetKeyCallback(window, physical_key_callback);
    glfwSetScrollCallback(window, scroll_callback);

    while (!glfwWindowShouldClose(window))
    {
        update_time_variables();
        /* printf("FPS: %.2f\n", fps_count); */

        clear_buffer();
        draw_into_buffer();
        swap_buffer(window);

        glfwPollEvents();
        /* update_player_transform(map_test); */
    }
    deinit();
    return EXIT_SUCCESS;
}

static GLFWwindow* init(char* title)
{
    GLFWwindow* window;

    window = get_window(title);
    man.shader_program = create_shader_program();
    if (!man.shader_program)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        return 0;
    }
    else
    {
        create_uniform();
        create_mesh();
        create_textures();
        initialize_maps();
        use_texture(man.tex[man.curr_tex]);
    }
    return window;
}

static void update_time_variables(void)
{
    static double last_time;
    double current_time;

    current_time = glfwGetTime();
    man.delta_time = current_time - last_time;
    last_time = current_time;
    man.fps_count = 1 / man.delta_time;
    return;
}

static void clear_buffer(void)
{
    /* Keep using glClear for when the window is resized */
    glClear(GL_COLOR_BUFFER_BIT);
    clear_drawing(man.tex[man.curr_tex]);
    return;
}

static void draw_into_buffer(void)
{
    draw_game();
    save_drawing(man.tex[man.curr_tex]);
    render_mesh();
    return;
}

static void swap_buffer(GLFWwindow* window)
{
    /*
        Keep using glfwSwapBuffers or the viewport remains black.
        It's the equivalent to mlx_put_image_to_window
    */
    man.curr_tex = (man.curr_tex + 1) % 2;
    glfwSwapBuffers(window);
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
}
