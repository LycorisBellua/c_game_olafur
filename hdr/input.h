#ifndef __INPUT_H__
#define __INPUT_H__

#ifndef __STDIO_H__
#define __STDIO_H__
#include <stdio.h>
#endif

#ifndef __GLFW_H__
#define __GLFW_H__
#include <GLFW/glfw3.h>
#endif

extern int movement_action[3];
extern int rotation_action;

void physical_key_callback(GLFWwindow* window, const int key, 
    const int scancode, const int action, const int mods);
void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);

#endif

