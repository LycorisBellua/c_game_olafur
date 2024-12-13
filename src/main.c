#include "cimmerian.h"

t_manager man;

static GLFWwindow *init(const char *title);
static void update_time_variables(void);
static void clear_buffer(void);
static void draw_into_buffer(t_map *m);
static void swap_buffer(GLFWwindow *window);
static void deinit(void);

int main(void)
{
    const char *title = "Cimmerian";
    GLFWwindow *window = init(title);
    
    if (!window)
        return EXIT_FAILURE;

    glfwSetKeyCallback(window, physical_key_callback);
    glfwSetScrollCallback(window, scroll_callback);

    reset_player_transform(man.map);
    man.player.pos.x = 22;
    man.player.pos.y = 12;

    while (!glfwWindowShouldClose(window))
    {
        update_time_variables();
        /* printf("FPS: %.2f\n", fps_count); */

        clear_buffer();
        draw_into_buffer(man.map);
        swap_buffer(window);

        glfwPollEvents();
        update_player_transform(man.map);
    }
    deinit();
    return EXIT_SUCCESS;
}

static GLFWwindow *init(const char *title)
{
    GLFWwindow *window;

    window = get_window(title);
    man.shader_program = create_shader_program();
    if (!window || !man.shader_program || !create_uniform() || !create_mesh() 
        || !create_frames() || !initialize_maps())
        return 0;
    man.tex_in_dof = 1.0; // [0.0 - 1.0] percentage
    use_frame(man.frame[man.curr_frame]);
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
    clear_drawing(man.frame[man.curr_frame]);
    return;
}

static void draw_into_buffer(t_map *m)
{
    draw_game(m);
    save_drawing(man.frame[man.curr_frame]);
    render_mesh();
    return;
}

static void swap_buffer(GLFWwindow *window)
{
    /*
        Keep using glfwSwapBuffers or the viewport remains black.
        It's the equivalent to mlx_put_image_to_window
    */
    man.curr_frame = (man.curr_frame + 1) % 2;
    glfwSwapBuffers(window);
    return;
}

static void deinit(void)
{
    glfwTerminate();
    free_shader_program();
    free_uniform();
    free_mesh();
    free_frames();
    free_maps();
    return;
}
