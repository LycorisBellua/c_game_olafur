#ifndef __COLORS_H__
#define __COLORS_H__

#ifndef __GLEW_H__
#define __GLEW_H__
#include <GL/glew.h>
#endif

/*
    The color format is RGBA. The color type is GLuint (unsigned int) and 
    the type of an individual RGBA channel is GLubyte (unsigned char).

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
extern GLuint* colors[NBR_COLORS];

void create_color_palette(void);
void set_color_from_hex_code(GLuint* color, const char* str);
void free_color_palette(void);

extern GLuint (*get_color_from_rgba)(const GLubyte r, const GLubyte g, 
    const GLubyte b, const GLubyte a);
extern void (*set_color_from_int)(GLuint* color, const GLuint value);
extern GLubyte (*get_red_channel)(const GLuint color);
extern GLubyte (*get_green_channel)(const GLuint color);
extern GLubyte (*get_blue_channel)(const GLuint color);
extern GLubyte (*get_alpha_channel)(const GLuint color);
extern void (*set_red_channel)(GLuint* color, const GLubyte value);
extern void (*set_green_channel)(GLuint* color, const GLubyte value);
extern void (*set_blue_channel)(GLuint* color, const GLubyte value);
extern void (*set_alpha_channel)(GLuint* color, const GLubyte value);

#endif

