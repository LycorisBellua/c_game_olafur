#include <stdio.h>
#include <GLFW/glfw3.h>
#include "../headers/input.h"

int get_local_key(int physical_key)
{
    /* It will be in lowercase, so -32 for uppercase */
    const char* key_name = glfwGetKeyName(physical_key, 0);
    return key_name[0] - 32;
}

void input_escape(GLFWwindow* window)
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
    glfwSetWindowShouldClose(window, GLFW_TRUE);
    return;
}

void input_up(int action)
{
    if (action == GLFW_PRESS)
        printf("Debug: UP started\n");
    else if (action == GLFW_REPEAT)
        printf("Debug: UP performed\n");
    else
        printf("Debug: UP canceled\n");
    return;
}

void input_down(int action)
{
    if (action == GLFW_PRESS)
        printf("Debug: DOWN started\n");
    else if (action == GLFW_REPEAT)
        printf("Debug: DOWN performed\n");
    else
        printf("Debug: DOWN canceled\n");
    return;
}

void input_right(int action)
{
    if (action == GLFW_PRESS)
        printf("Debug: RIGHT started\n");
    else if (action == GLFW_REPEAT)
        printf("Debug: RIGHT performed\n");
    else
        printf("Debug: RIGHT canceled\n");
    return;
}

void input_left(int action)
{
    if (action == GLFW_PRESS)
        printf("Debug: LEFT started\n");
    else if (action == GLFW_REPEAT)
        printf("Debug: LEFT performed\n");
    else
        printf("Debug: LEFT canceled\n");
    return;
}

void input_strafe_left(int action)
{
    if (action == GLFW_PRESS)
        printf("Debug: STRAFE LEFT started\n");
    else if (action == GLFW_REPEAT)
        printf("Debug: STRAFE LEFT performed\n");
    else
        printf("Debug: STRAFE LEFT canceled\n");
    return;
}

void input_strafe_right(int action)
{
    if (action == GLFW_PRESS)
        printf("Debug: STRAFE RIGHT started\n");
    else if (action == GLFW_REPEAT)
        printf("Debug: STRAFE RIGHT performed\n");
    else
        printf("Debug: STRAFE RIGHT canceled\n");
    return;
}

void physical_key_callback
(
    GLFWwindow* window, 
    int key, 
    __attribute__((unused))int scancode, 
    int action, 
    __attribute__((unused))int mods
)
{
    if (key > 255)
    {
        if (key == GLFW_KEY_ESCAPE)
            input_escape(window);
        else if (key == GLFW_KEY_UP)
            input_up(action);
        else if (key == GLFW_KEY_DOWN)
            input_down(action);
        else if (key == GLFW_KEY_RIGHT)
            input_right(action);
        else if (key == GLFW_KEY_LEFT)
            input_left(action);
    }
    else if (key >= 'A' && key <= 'Z')
    {
        printf("Local key: %c\n", get_local_key(key));

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

void char_key_callback
(
    __attribute__((unused))GLFWwindow* window, 
    unsigned int codepoint
)
{
    /*
        UTF-8 characters such as diacritics and Japanese handled 
        thanks to %lc, but -pedantic GCC warning [-Wformat]:
        "ISO C90 does not support the ‘%lc’ gnu_printf format"
    */
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wformat"
    printf("Debug (char callback): %lc\n", codepoint);
    #pragma GCC diagnostic pop
    return;
}

