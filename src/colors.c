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

Color get_color_hex(const char* str, const GLubyte alpha)
{
    int i;
    Color c;

    i = str[0] == '#';
    c.r = hex_char_to_int(str[i + 0]) * 16 + hex_char_to_int(str[i + 1]);
    c.g = hex_char_to_int(str[i + 2]) * 16 + hex_char_to_int(str[i + 3]);
    c.b = hex_char_to_int(str[i + 4]) * 16 + hex_char_to_int(str[i + 5]);
    c.a = alpha;
    return c;
}

Color get_alpha_blended_color(Color prev, Color new)
{
    Color blend;

    blend.a = new.a + (255 - new.a) * prev.a / 255;
    if (!blend.a)
        return get_color_rgba(0, 0, 0, 0);
    blend.r = (new.a * new.r + (255 - new.a) * prev.a * prev.r / 255) / blend.a;
    blend.g = (new.a * new.g + (255 - new.a) * prev.a * prev.g / 255) / blend.a;
    blend.b = (new.a * new.b + (255 - new.a) * prev.a * prev.b / 255) / blend.a;
    return blend;
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
    return 0;
}
