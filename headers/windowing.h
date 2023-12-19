#ifndef __WINDOWING_H__
#define __WINDOWING_H__

float get_aspect_ratio(void);

GLFWwindow* get_window(const char* title);
void toggle_fullscreen(GLFWwindow* window);

#endif

