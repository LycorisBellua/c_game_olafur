#include "cimmerian.h"

t_manager man;

static GLFWwindow* init(char* title);
static void update_time_variables(void);
static void draw(void);
static void deinit(void);

int main(void)
{
    char* title = "Cimmerian";
    GLFWwindow* window = init(title);
    
    if (!window)
        return EXIT_FAILURE;

    glfwSetKeyCallback(window, physical_key_callback);
    glfwSetScrollCallback(window, scroll_callback);

    man.font_default = load_sprite("assets/font_peaberry_white_outline.png");
    man.font_title = load_sprite("assets/font_peaberry_shiny.png");

    while (!glfwWindowShouldClose(window))
    {
        update_time_variables();
        /* printf("FPS: %.2f\n", fps_count); */

        /* Keep using glClear for when the window is resized */
        glClear(GL_COLOR_BUFFER_BIT);
        clear_drawing(man.tex[man.curr_tex]);
        draw();

        /*
            Keep using glfwSwapBuffers or the viewport remains black.
            It's the equivalent to mlx_put_image_to_window
        */
        man.curr_tex = (man.curr_tex + 1) % 2;
        glfwSwapBuffers(window);

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
        initialize_interfaces();
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

static void draw(void)
{
    man.active_interface->draw();
    save_drawing(man.tex[man.curr_tex]);
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
    free_sprite(man.font_default);
    free_sprite(man.font_title);
    return;
}
