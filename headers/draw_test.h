#ifndef __DRAW_TEST_H__
#define __DRAW_TEST_H__

#ifndef __GLEW_H__
#define __GLEW_H__
#include <GL/glew.h>
#endif

#include "textures.h"

void draw_test_corners(Texture* t);
void draw_test_center(Texture* t);
void draw_test_lines(Texture* t);
void draw_test_rectangles(Texture* t);
void draw_test_circles(Texture* t);
void draw_test_shapes(Texture* t);
void draw_test_gradient_line(Texture* t);
void draw_test_gradient(Texture* t);

void draw_8bit_rgb_palette(Texture* t);
void draw_8bit_rgba_palette(Texture* t);

#endif

