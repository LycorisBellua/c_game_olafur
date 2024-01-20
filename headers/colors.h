#ifndef __COLORS_H__
#define __COLORS_H__

#ifndef __GLEW_H__
#define __GLEW_H__
#include <GL/glew.h>
#endif

/*
    The color format is RGBA.
    GLubyte is an unsigned char and represents one RGBA channel.

    If you want the default color, do `colors[0]`.
*/

typedef enum
{
    COLOR_WHITE,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_BLUE
} ColorName;

#define NBR_COLORS 4+1
extern GLubyte* colors[NBR_COLORS];

void create_color_palette(void);
void free_color_palette(void);
void set_color_from_hex_code(GLubyte* color, const char* str);
void set_color_from_other_color(GLubyte* dst, const GLubyte* src);
void darken_color(GLubyte* color, const int percentage);
void lighten_color(GLubyte* color, const int percentage);

#endif

