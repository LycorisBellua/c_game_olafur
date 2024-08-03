#ifndef __WINDOWING_H__
#define __WINDOWING_H__

#ifndef __STDIO_H__
#define __STDIO_H__
#include <stdio.h>
#endif

#ifndef __STDLIB_H__
#define __STDLIB_H__
#include <stdlib.h>
#endif

#ifndef __GLFW_H__
#define __GLFW_H__
#include <GLFW/glfw3.h>
#endif

typedef struct
{
    int monitor_width;
    int monitor_height;
    float aspect_ratio;

    int window_width_default;
    int window_height_default;
    int window_width;
    int window_height;
    int window_position_x;
    int window_position_y;
    int fullscreen_x;
    int fullscreen_y;
} Resolution;

extern Resolution res;

GLFWwindow* get_window(const char* title);
void toggle_fullscreen(GLFWwindow* window);

#endif

