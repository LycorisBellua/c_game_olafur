#ifndef __INTERFACES_H__
#define __INTERFACES_H__

#ifndef __GLEW_H__
#define __GLEW_H__
#include <GL/glew.h>
#endif

#ifndef __GLFW_H__
#define __GLFW_H__
#include <GLFW/glfw3.h>
#endif

typedef void (*DrawFunction)(void);
typedef void (*NavigationUI)(const int dir);
typedef struct Interface Interface;

struct Interface
{
    DrawFunction draw;
    Interface* previous;
    Interface* next;
};

extern Interface* active_interface;
extern float global_x, global_y;
extern NavigationUI nav_ui_horizontal, nav_ui_vertical;

void initialize_interfaces(void);
void set_active_interface(Interface* interface);
void update_global_coordinates(void);
void nav_ui_confirm(GLFWwindow* window);
void nav_ui_cancel(GLFWwindow* window);

#endif

