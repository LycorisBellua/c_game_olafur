#ifndef __DRAW_H__
#define __DRAW_H__

#ifndef __STRING_H__
#define __STRING_H__
#include <string.h>
#endif

#ifndef __GLEW_H__
#define __GLEW_H__
#include <GL/glew.h>
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
    GLubyte* color;
} Vertex;

int get_coord_x(Texture* t, const float normalized);
int get_coord_y(Texture* t, const float normalized);

void draw_point(Texture* t, GLubyte* color, int x, int y);
void draw_line(Texture* t, Vertex v1, Vertex v2);
void draw_rectangle(Texture* t, const int full, Vertex v, int width, 
    int height);
void draw_circle(Texture* t, const int full, Vertex v, const int radius);
void draw_polygon(Texture* t, const int full, ...);

void draw_test_corners(Texture* t);
void draw_test_center(Texture* t);
void draw_test_lines(Texture* t);
void draw_test_rectangles(Texture* t);
void draw_test_circles(Texture* t);
void draw_test_gradient(Texture* t);

#endif

