#ifndef __COLORS_H__
#define __COLORS_H__

typedef struct
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} Color;

typedef enum
{
    COLOR_BLACK,
    COLOR_WHITE,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_BLUE
} ColorName;

#define NBR_COLORS 6
extern Color** color_default;
extern Color* colors[NBR_COLORS];

void create_color_palette(void);
void free_color_palette(void);
void set_color_from_hex_code(Color* color, const char* str);
void set_color_from_other_color(Color* dst, const Color* src);
void set_pixel_color(unsigned char* pixel, const Color* color);
void darken_color(Color* color, const int percentage);
void lighten_color(Color* color, const int percentage);

#endif

