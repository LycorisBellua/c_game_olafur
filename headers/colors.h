#ifndef __COLORS_H__
#define __COLORS_H__

#ifndef __GLEW_H__
#define __GLEW_H__
#include <GL/glew.h>
#endif

/*
    The color format is RGB332. This means only one byte, composed of 3 bits 
    for the red channel, 3 bits for the green one, and 2 bits for blue. It 
    makes for a limited palette in comparison to using 4 bytes per pixel, but 
    it remains pleasant looking and it's a nice challenge.

    If you want the default color, which is white, do `colors[0]`.
*/

typedef enum
{
    COLOR_WHITE,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_BLUE
} ColorName;

#define NBR_COLORS 4+1
extern GLubyte colors[NBR_COLORS];

void create_color_palette(void);

GLubyte get_color_from_hex_code(const char* str);
GLubyte get_color_from_rgb(GLubyte r_3bits, GLubyte g_3bits, GLubyte b_2bits);
GLubyte get_red_channel(const GLubyte color);
GLubyte get_green_channel(const GLubyte color);
GLubyte get_blue_channel(const GLubyte color);
void set_red_channel(GLubyte* color, GLubyte value_3bits);
void set_green_channel(GLubyte* color, GLubyte value_3bits);
void set_blue_channel(GLubyte* color, GLubyte value_2bits);

#endif

