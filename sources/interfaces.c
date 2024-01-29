#include "../headers/interfaces.h"
#include "../headers/textures.h"
#include "../headers/input.h"
#include "../headers/time.h"
#include "../headers/draw.h"
#include "../headers/draw_test.h"

Interface* active_interface = {0};
static Interface main_menu_interface;
static Interface game_interface;
static void draw_main_menu(void);
static void draw_game(void);

float global_x, global_y;
static void reset_global_coordinates(void);

NavigationUI nav_ui_horizontal, nav_ui_vertical;
void nav_ui_confirm(GLFWwindow* window);
void nav_ui_cancel(GLFWwindow* window);
static int index_btn_selected = 0;
static void update_index_btn_selected_horizontal(const int dir);
static void update_index_btn_selected_vertical(const int dir);

void initialize_interfaces(void)
{
    set_active_interface(&main_menu_interface);

    main_menu_interface.draw = draw_main_menu;
    main_menu_interface.previous = 0;
    main_menu_interface.next = &game_interface;

    game_interface.draw = draw_game;
    game_interface.previous = &main_menu_interface;
    game_interface.next = 0;

    nav_ui_horizontal = &update_index_btn_selected_horizontal;
    nav_ui_vertical = &update_index_btn_selected_vertical;
    return;
}

void set_active_interface(Interface* interface)
{
    active_interface = interface;
    reset_global_coordinates();
    index_btn_selected = 0;
    return;
}

void update_global_coordinates(void)
{
    const float speed = 50.0f;
    global_x += movement_action[0] * speed * delta_time;
    global_y += movement_action[2] * speed * delta_time;
    return;
}

static void reset_global_coordinates(void)
{
    global_x = global_y = 0;
    return;
}

static void update_index_btn_selected_horizontal(const int dir)
{
    if (active_interface != &main_menu_interface)
        return;

    /* [0-2] range because 3 buttons */
    index_btn_selected = (index_btn_selected + dir + 3) % 3;
    return;
}

static void update_index_btn_selected_vertical(const int dir)
{
    if (active_interface != &main_menu_interface)
        return;

    /* [0-2] range because 3 buttons */
    index_btn_selected = (index_btn_selected - dir + 3) % 3;
    return;
}

void nav_ui_confirm(GLFWwindow* window)
{
    if (active_interface != &main_menu_interface)
        return;

    if (index_btn_selected == 0)
    {
        set_active_interface(&game_interface);
    }
    else if (index_btn_selected == 1)
    {
        printf("DEBUG: \"Options\" sub-menu not yet implemented.\n");
    }
    else if (index_btn_selected == 2)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    return;
}

void nav_ui_cancel(GLFWwindow* window)
{
    if (!active_interface->previous)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    set_active_interface(active_interface->previous);
    return;
}

static void draw_main_menu(void)
{
    const Vector btn_size = { 150, 50 };
    VectorF btn_size_norm;
    Vertex btn[3];
    const char* btn_name[3] = { "Start Game", "Options", "Quit" };
    GLubyte color_default, color_selected;

    btn_size_norm.x = get_coord_x_norm(TEX_MAIN, btn_size.x-1);
    btn_size_norm.y = get_coord_x_norm(TEX_MAIN, btn_size.y-1);
    color_default = colors[40];
    color_selected = colors[9];

    draw_test_gradient(TEX_MAIN);

    /* "Start Game" --------------------------------------------------------- */
    btn[0].coords.x = get_coord_x(TEX_MAIN, 0.5f - btn_size_norm.x/2) + 1;
    btn[0].coords.y = get_coord_y(TEX_MAIN, 0.65f - btn_size_norm.y/2) - 1;

    if (index_btn_selected == 0)
    {
        btn[0].color = 0xFF;
        draw_rectangle(TEX_MAIN, 0, btn[0], btn_size.x, btn_size.y);
        btn[0].color = color_selected;
    }
    else
        btn[0].color = color_default;

    --btn[0].coords.x;
    ++btn[0].coords.y;
    draw_rectangle(TEX_MAIN, 1, btn[0], btn_size.x, btn_size.y);

    btn[0].color = 0;
    draw_rectangle(TEX_MAIN, 0, btn[0], btn_size.x, btn_size.y);

    /* "Options" ------------------------------------------------------------ */
    btn[1].coords.x = get_coord_x(TEX_MAIN, 0.5f - btn_size_norm.x/2) + 1;
    btn[1].coords.y = get_coord_y(TEX_MAIN, 0.4f - btn_size_norm.y/2) - 1;

    if (index_btn_selected == 1)
    {
        btn[1].color = 0xFF;
        draw_rectangle(TEX_MAIN, 0, btn[1], btn_size.x, btn_size.y);
        btn[1].color = color_selected;
    }
    else
        btn[1].color = color_default;

    --btn[1].coords.x;
    ++btn[1].coords.y;
    draw_rectangle(TEX_MAIN, 1, btn[1], btn_size.x, btn_size.y);

    btn[1].color = 0;
    draw_rectangle(TEX_MAIN, 0, btn[1], btn_size.x, btn_size.y);

    /* "Quit" --------------------------------------------------------------- */
    btn[2].coords.x = get_coord_x(TEX_MAIN, 0.5f - btn_size_norm.x/2) + 1;
    btn[2].coords.y = get_coord_y(TEX_MAIN, 0.15f - btn_size_norm.y/2) - 1;

    if (index_btn_selected == 2)
    {
        btn[2].color = 0xFF;
        draw_rectangle(TEX_MAIN, 0, btn[2], btn_size.x, btn_size.y);
        btn[2].color = color_selected;
    }
    else
        btn[2].color = color_default;

    --btn[2].coords.x;
    ++btn[2].coords.y;
    draw_rectangle(TEX_MAIN, 1, btn[2], btn_size.x, btn_size.y);

    btn[2].color = 0;
    draw_rectangle(TEX_MAIN, 0, btn[2], btn_size.x, btn_size.y);
    return;
}

static void draw_game(void)
{
    draw_test_rectangles(TEX_MAIN);
    draw_test_circles(TEX_MAIN);
    draw_test_lines(TEX_MAIN);
    draw_test_corners(TEX_MAIN);
    draw_test_center(TEX_MAIN);
    draw_test_gradient_line(TEX_MAIN);
    return;
}

