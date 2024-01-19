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

typedef struct
{
    Vector coords;
} Vertex;

/* `int x, int y` here, so that either Vector or VectorF can be used */
void draw_point(Texture* t, GLubyte* color, int x, int y);

void draw_line(Texture* t, Vertex v1, Vertex v2);
void draw_rectangle(Texture* t, const int is_filled, Vertex v, int width, 
    int height);
void draw_circle(Texture* t, const int filled_up, Vertex v, int radius);
void draw_polygon(Texture* t, const int filled_up, ...);

void draw_test_corners(Texture* t);
void draw_test_center(Texture* t);
void draw_test_lines(Texture* t);
void draw_test_rectangles(Texture* t);
void draw_test_gradient(Texture* t);

#endif

