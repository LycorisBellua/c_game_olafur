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

typedef struct Interface Interface;
struct Interface
{
    int nav_ui;
    void (*draw)(void);
    void (*reset_input)(void);
    Interface* previous;
    Interface* next;
};

extern Interface* active_interface;

void initialize_interfaces(void);
void set_active_interface(Interface* interface);

#endif

