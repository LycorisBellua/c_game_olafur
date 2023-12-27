#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#ifndef __GLEW_H__
#define __GLEW_H__
#include <GL/glew.h>
#endif

extern float player_speed[3];

void move_player(void);
void straighten_pitch(void);
void set_pitch_back(void);

#endif

