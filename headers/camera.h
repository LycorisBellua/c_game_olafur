#ifndef __CAMERA_H__
#define __CAMERA_H__

extern int camera_moves;
extern float camera_transform[9];
extern float camera_speed[3];

void deactivate_camera_transform(void);
void move_camera(void);

#endif

