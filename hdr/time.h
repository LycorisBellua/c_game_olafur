#ifndef __TIME_H__
#define __TIME_H__

#ifndef __GLFW_H__
#define __GLFW_H__
#include <GLFW/glfw3.h>
#endif

extern float delta_time;
extern float fps_count;

void update_time_variables(void);

#endif

