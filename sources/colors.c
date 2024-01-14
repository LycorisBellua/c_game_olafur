#include "../headers/colors.h"
#include "../headers/file_handling.h"
#include "../headers/maths.h"

void set_color_from_hex_string(Color* color, const char* str)
{
    /* e.g.: "#097e7bff" - "#097e7b" - "097e7b" */
    int i = str[0] == '#';

    color->r = hex_char_to_int(str[i+0])*16 + hex_char_to_int(str[i+1]);
    color->g = hex_char_to_int(str[i+2])*16 + hex_char_to_int(str[i+3]);
    color->b = hex_char_to_int(str[i+4])*16 + hex_char_to_int(str[i+5]);

    color->a = !str[i+6] ? 255 
        : hex_char_to_int(str[i+6])*16 + hex_char_to_int(str[i+7]);
    return;
}

void darken_color(Color* color, const int percentage)
{
    const int perc_to_rgb = 255/100 * percentage;

    color->r = CLAMP(color->r - perc_to_rgb, 0, 255);
    color->g = CLAMP(color->g - perc_to_rgb, 0, 255);
    color->b = CLAMP(color->b - perc_to_rgb, 0, 255);
    return;
}

void lighten_color(Color* color, const int percentage)
{
    const int perc_to_rgb = 255/100 * percentage;

    color->r = CLAMP(color->r + perc_to_rgb, 0, 255);
    color->g = CLAMP(color->g + perc_to_rgb, 0, 255);
    color->b = CLAMP(color->b + perc_to_rgb, 0, 255);
    return;
}

