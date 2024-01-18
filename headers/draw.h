#ifndef __DRAW_H__
#define __DRAW_H__

#ifndef __STRING_H__
#define __STRING_H__
#include <string.h>
#endif

#include "textures.h"

/* 
    The origin of the coordinate system is in the bottom left hand corner.
    The X axis range is [0, t->width] and the Y axis range is [0, t->height].
*/

void draw_point(Texture* t, int x, int y);
void draw_line(Texture* t, int x1, int y1, int x2, int y2);
void draw_rectangle(Texture* t, const int is_filled, int x, int y, int width, 
    int height);
void draw_circle(Texture* t, const int filled_up, int x, int y, int radius);
void draw_polygon(Texture* t, const int filled_up, ...);

#endif

