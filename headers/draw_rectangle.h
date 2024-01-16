#ifndef __DRAW_RECTANGLE_H__
#define __DRAW_RECTANGLE_H__

#include "textures.h"

/* Coord is top left hand corner of shape */
void draw_rectangle(Texture* t, const int is_filled, int x, int y, int width, 
    int height);

#endif

