#ifndef __COORDS_H__
#define __COORDS_H__

/* 
    The origin of the coordinate system is in the bottom left hand corner.
    The X axis range is [0, t->width] and the Y axis range is [0, t->height].
*/

#include "textures.h"

typedef struct
{
    int x;
    int y;
} Vector;

typedef struct
{
    float x;
    float y;
} VectorF;

int get_coord_x(Texture* t, float normalized);
int get_coord_y(Texture* t, float normalized);
float get_coord_x_norm(Texture* t, int coord);
float get_coord_y_norm(Texture* t, int coord);
int is_coord_out_of_bounds(int axis_length, int coord);
Vector get_direction(Vector v1, Vector v2);
Vector get_direction_unsafe(Vector v1, Vector v2);
VectorF get_direction_float(VectorF v1, VectorF v2);
float get_distance(const VectorF a, const VectorF b);

#endif

