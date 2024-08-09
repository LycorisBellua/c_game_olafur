#include "cimmerian.h"

Color get_color_from_hex_code(const char* str)
{
    /* e.g.: "#097e7b" - "097e7b" */
    const int i = str[0] == '#';
    Color color;

    color.r = hex_char_to_int(str[i+0])*16 + hex_char_to_int(str[i+1]);
    color.g = hex_char_to_int(str[i+2])*16 + hex_char_to_int(str[i+3]);
    color.b = hex_char_to_int(str[i+4])*16 + hex_char_to_int(str[i+5]);
    return color;
}

Color get_color(const GLubyte r, const GLubyte g, const GLubyte b,
    const GLubyte a)
{
    Color c;

    c.r = r;
    c.g = g;
    c.b = b;
    c.a = a;
    return c;
}
