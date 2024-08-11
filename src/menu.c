#include "cimmerian.h"

static int index_btn_selected = 0;

void draw_main_menu(void)
{
    const Vector btn_size = { 150, 50 };
    VectorF btn_size_norm;
    Vertex btn[3];
    /* const char* btn_name[3] = { "Start Game", "Options", "Quit" }; */
    Color color_default, color_selected;
    Texture *t = man.tex[man.curr_tex];

    btn_size_norm.x = get_coord_x_norm(t, btn_size.x-1);
    btn_size_norm.y = get_coord_x_norm(t, btn_size.y-1);
    color_default = get_color_hex("#493B4C", 255);
    color_selected = get_color_hex("#F59B91", 255);

    draw_test_gradient(t);

    /* "Start Game" --------------------------------------------------------- */
    btn[0].coords.x = get_coord_x(t, 0.5f - btn_size_norm.x/2) + 1;
    btn[0].coords.y = get_coord_y(t, 0.2f - btn_size_norm.y/2) + 1;

    if (index_btn_selected == 0)
    {
        btn[0].color = get_color_rgba(0xFF, 0xFF, 0xFF, 0xFF);
        draw_rectangle(t, btn[0], btn_size);
        btn[0].color = color_selected;
    }
    else
        btn[0].color = color_default;

    --btn[0].coords.x;
    --btn[0].coords.y;
    draw_rectangle_full(t, btn[0], btn_size);

    btn[0].color = get_color_rgba(0x00, 0x00, 0x00, 0xFF);
    draw_rectangle(t, btn[0], btn_size);

    /* "Options" ------------------------------------------------------------ */
    btn[1].coords.x = get_coord_x(t, 0.5f - btn_size_norm.x/2) + 1;
    btn[1].coords.y = get_coord_y(t, 0.45f - btn_size_norm.y/2) + 1;

    if (index_btn_selected == 1)
    {
        btn[1].color = get_color_rgba(0xFF, 0xFF, 0xFF, 50);
        draw_rectangle(t, btn[1], btn_size);
        btn[1].color = color_selected;
    }
    else
        btn[1].color = color_default;
    btn[1].color.a = 50;

    --btn[1].coords.x;
    --btn[1].coords.y;
    draw_rectangle_full(t, btn[1], btn_size);

    btn[1].color = get_color_rgba(0x00, 0x00, 0x00, 50);
    draw_rectangle(t, btn[1], btn_size);

    /* "Quit" --------------------------------------------------------------- */
    btn[2].coords.x = get_coord_x(t, 0.5f - btn_size_norm.x/2) + 1;
    btn[2].coords.y = get_coord_y(t, 0.70f - btn_size_norm.y/2) + 1;

    if (index_btn_selected == 2)
    {
        btn[2].color = get_color_rgba(0xFF, 0xFF, 0xFF, 0xFF);
        draw_rectangle(t, btn[2], btn_size);
        btn[2].color = color_selected;
    }
    else
        btn[2].color = color_default;

    --btn[2].coords.x;
    --btn[2].coords.y;
    draw_rectangle_full(t, btn[2], btn_size);

    btn[2].color = get_color_rgba(0x00, 0x00, 0x00, 0xFF);
    draw_rectangle(t, btn[2], btn_size);
    return;
}

void reset_menu_button_selection(void)
{
    index_btn_selected = 0;
    return;
}

void nav_ui_horizontal(const int dir)
{
    if (!man.active_interface->nav_ui)
        return;

    /* [0-2] range because 3 buttons */
    index_btn_selected = (index_btn_selected + dir + 3) % 3;
    return;
}

void nav_ui_vertical(const int dir)
{
    if (!man.active_interface->nav_ui)
        return;

    /* [0-2] range because 3 buttons */
    index_btn_selected = (index_btn_selected - dir + 3) % 3;
    return;
}

void nav_ui_confirm(GLFWwindow* window)
{
    if (!man.active_interface->nav_ui)
        return;

    if (index_btn_selected == 0)
    {
        if (man.active_interface->next)
            set_active_interface(man.active_interface->next);
    }
    else if (index_btn_selected == 1)
    {
        printf("DEBUG: \"Options\" button pressed, but feature not yet "
            "implemented.\n");
    }
    else if (index_btn_selected == 2)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    return;
}

void nav_ui_cancel(GLFWwindow* window)
{
    if (!man.active_interface->previous)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    set_active_interface(man.active_interface->previous);
    return;
}
