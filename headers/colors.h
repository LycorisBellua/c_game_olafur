#ifndef __COLORS_H__
#define __COLORS_H__

typedef struct
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} Color;

void set_color_from_hex_string(Color* color, const char* str);
void darken_color(Color* color, const int percentage);
void lighten_color(Color* color, const int percentage);

#endif

