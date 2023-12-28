#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#ifndef __GLEW_H__
#define __GLEW_H__
#include <GL/glew.h>
#endif

extern int zero_player_transform;
extern float player_speed[3];

void move_player(void);
void deactivate_player_transform(void);

#endif

