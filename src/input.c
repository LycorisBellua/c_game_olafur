#include "cimmerian.h"

int movement_action[3] = {0};
static int movement_input[3] = {0};
static void update_movement_action(const int axis);

int rotation_action = 0;
static int rotation_input = 0;
static void update_rotation_action(void);

/*
static int get_local_key(const int physical_key);
*/
static void input_escape(GLFWwindow* window, const int action);
static void input_enter(GLFWwindow* window, const int action);

static void input_up(const int action);
static void input_down(const int action);
static void input_right(const int action);
static void input_left(const int action);
static void input_strafe_left(const int action);
static void input_strafe_right(const int action);

void physical_key_callback
(
    GLFWwindow* window, 
    const int key, 
    __attribute__((unused))const int scancode, 
    const int action, 
    __attribute__((unused))const int mods
)
{
    if (key > 255)
    {
        if (key == GLFW_KEY_ESCAPE)
            input_escape(window, action);
        else if (key == GLFW_KEY_ENTER)
            input_enter(window, action);
        else if (key == GLFW_KEY_UP)
            input_up(action);
        else if (key == GLFW_KEY_DOWN)
            input_down(action);
        else if (key == GLFW_KEY_RIGHT)
            input_right(action);
        else if (key == GLFW_KEY_LEFT)
            input_left(action);
        else if (key == GLFW_KEY_F11 && action == GLFW_PRESS)
            toggle_fullscreen(window);
    }
    else if (key >= 'A' && key <= 'Z')
    {
        if (key == GLFW_KEY_W)
            input_up(action);
        else if (key == GLFW_KEY_S)
            input_down(action);
        else if (key == GLFW_KEY_D)
            input_right(action);
        else if (key == GLFW_KEY_A)
            input_left(action);
        else if (key == GLFW_KEY_Q)
            input_strafe_left(action);
        else if (key == GLFW_KEY_E)
            input_strafe_right(action);
    }
    return;
}

void scroll_callback
(
    __attribute__((unused))GLFWwindow* window, 
    __attribute__((unused))double x_offset, 
    double y_offset
)
{
    set_minimap_display(y_offset);
    return;
}

static void update_movement_action(const int axis)
{
    int input = movement_input[axis];

    if (input < 0)
        movement_action[axis] = -1;
    else if (input > 0)
        movement_action[axis] = 1;
    else
        movement_action[axis] = 0;
    return;
}

static void update_rotation_action(void)
{
    if (rotation_input < 0)
        rotation_action = -1;
    else if (rotation_input > 0)
        rotation_action = 1;
    else
        rotation_action = 0;
    return;
}

/*
static int get_local_key(const int physical_key)
{
    // It will be in lowercase, so -32 for uppercase
    const char* key_name = glfwGetKeyName(physical_key, 0);
    return key_name[0] - 32;
}
*/

static void input_escape(GLFWwindow* window, const int action)
{
    /* 
       By default, the window closing event is triggered by the cross 
       window button or Alt+F4. We also decide to set the Escape key 
       as such.

       Any of these closing inputs will set 
       `glfwWindowShouldClose(window)` to GLFW_TRUE, after which the 
       window can be closed with `glfwDestroyWindow(window)`, or the 
       whole program (windows included) can be closed with 
       `glfwTerminate()`.
    */

    if (action != GLFW_PRESS)
        return;
    nav_ui_cancel(window);
    return;
}

static void input_enter(GLFWwindow* window, const int action)
{
    if (action != GLFW_PRESS)
        return;
    nav_ui_confirm(window);
    return;
}

static void input_up(const int action)
{
    if (action == GLFW_PRESS)
    {
        movement_input[2] += 1;
        nav_ui_vertical(1);
    }
    else if (action == GLFW_REPEAT)
    {
    }
    else
    {
        movement_input[2] -= 1;
    }

    update_movement_action(2);
    return;
}

static void input_down(const int action)
{
    if (action == GLFW_PRESS)
    {
        movement_input[2] += -1;
        nav_ui_vertical(-1);
    }
    else if (action == GLFW_REPEAT)
    {
    }
    else
    {
        movement_input[2] -= -1;
    }

    update_movement_action(2);
    return;
}

static void input_right(const int action)
{
    if (action == GLFW_PRESS)
    {
        rotation_input += 1;
        nav_ui_horizontal(1);
    }
    else if (action == GLFW_REPEAT)
    {
    }
    else
    {
        rotation_input -= 1;
    }

    update_rotation_action();
    return;
}

static void input_left(const int action)
{
    if (action == GLFW_PRESS)
    {
        rotation_input += -1;
        nav_ui_horizontal(-1);
    }
    else if (action == GLFW_REPEAT)
    {
    }
    else
    {
        rotation_input -= -1;
    }

    update_rotation_action();
    return;
}

static void input_strafe_left(const int action)
{
    if (action == GLFW_PRESS)
    {
        movement_input[0] += -1;
        nav_ui_horizontal(-1);
    }
    else if (action == GLFW_REPEAT)
    {
    }
    else
    {
        movement_input[0] -= -1;
    }

    update_movement_action(0);
    return;
}

static void input_strafe_right(const int action)
{
    if (action == GLFW_PRESS)
    {
        movement_input[0] += 1;
        nav_ui_horizontal(1);
    }
    else if (action == GLFW_REPEAT)
    {
    }
    else
    {
        movement_input[0] -= 1;
    }

    update_movement_action(0);
    return;
}
