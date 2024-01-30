#ifndef __MENU_H__
#define __MENU_H__

#ifndef __GLEW_H__
#define __GLEW_H__
#include <GL/glew.h>
#endif

#ifndef __GLFW_H__
#define __GLFW_H__
#include <GLFW/glfw3.h>
#endif

/* Called from the "main menu" interface object */
void draw_main_menu(void);
void reset_menu_button_selection(void);

/* Events called from input.c */
void nav_ui_horizontal(const int dir);
void nav_ui_vertical(const int dir);
void nav_ui_confirm(GLFWwindow* window);
void nav_ui_cancel(GLFWwindow* window);

#endif

