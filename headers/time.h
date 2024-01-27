#ifndef __TIME_H__
#define __TIME_H__

/*
#ifndef __GLEW_H__
#define __GLEW_H__
#include <GL/glew.h>
#endif
*/

#ifndef __GLFW_H__
#define __GLFW_H__
#include <GLFW/glfw3.h>
#endif

#define FPS (1/get_delta_time())

float get_delta_time(void);

#endif

