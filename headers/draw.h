#ifndef __DRAW_H__
#define __DRAW_H__

#ifndef __GLEW_H__
#define __GLEW_H__
#include <GL/glew.h>
#endif

#include "textures.h"
#include "colors.h"

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
    GLubyte color;
} Vertex;

int get_coord_x(Texture* t, float normalized);
int get_coord_y(Texture* t, float normalized);
float get_coord_x_norm(Texture* t, int coord);
float get_coord_y_norm(Texture* t, int coord);
int is_coord_out_of_bounds(int axis_length, int coord);
Vector get_direction(Vector v1, Vector v2);

void draw_point(Texture* t, GLubyte color, int x, int y);
void draw_line(Texture* t, Vertex v1, Vertex v2);
void draw_line_horizontal(Texture* t, Vertex v, int last_x);
void draw_line_vertical(Texture* t, Vertex v, int last_y);
void draw_line_diagonal(Texture* t, Vertex v, Vector dir);
void draw_rectangle(Texture* t, int full, Vertex v, int width, int height);
void draw_circle(Texture* t, int full, Vertex v, int radius);
void draw_shape(Texture* t, int full, Vertex arr[], int len);

#endif

