#include "cimmerian.h"

static int is_lower(const int c);
static int is_upper(const int c);
static int hex_char_to_int(const char c);

Color get_color_rgba(const GLubyte r, const GLubyte g, const GLubyte b,
    const GLubyte a)
{
    Color c;

    c.r = r;
    c.g = g;
    c.b = b;
    c.a = a;
    return c;
}

Color get_color_hex(const char* str)
{
    const int i = str[0] == '#';
    Color color;

    color.r = hex_char_to_int(str[i+0])*16 + hex_char_to_int(str[i+1]);
    color.g = hex_char_to_int(str[i+2])*16 + hex_char_to_int(str[i+3]);
    color.b = hex_char_to_int(str[i+4])*16 + hex_char_to_int(str[i+5]);
    color.a = 255;
    return color;
}

int is_digit(const int c)
{
    return c >= '0' && c <= '9';
}

static int is_lower(const int c)
{
    return c >= 'a' && c <= 'z';
}

static int is_upper(const int c)
{
    return c >= 'A' && c <= 'Z';
}

static int hex_char_to_int(const char c)
{
    if (is_digit(c))
        return c - '0';
    else if (is_lower(c))
        return c > 'f' ? 0 : 10 + (c - 'a');
    else if (is_upper(c))
        return c > 'F' ? 0 : 10 + (c - 'A');
    else
        return 0;
}
