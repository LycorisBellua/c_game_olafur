#include "../headers/colors.h"
#include "../headers/file_handling.h"
#include "../headers/maths.h"

Color** color_default = 0;
Color* colors[NBR_COLORS] = {0};

static Color* create_color(const char* hex_code);

void create_color_palette(void)
{
    color_default = &colors[COLOR_WHITE];
    colors[COLOR_BLACK] = create_color("#000000");
    colors[COLOR_WHITE] = create_color("#FFFFFF");
    colors[COLOR_RED] = create_color("#FF0000");
    colors[COLOR_GREEN] = create_color("#00FF00");
    colors[COLOR_BLUE] = create_color("#0000FF");
    colors[NBR_COLORS - 1] = 0;
    return;
}

void free_color_palette(void)
{
    unsigned int i;
    for (i = 0; i < sizeof(colors)/sizeof(Color*); ++i)
    {
        if (!colors[i])
            break;

        free(colors[i]);
        /* Nullify the reference to prevent a double free */
        colors[i] = 0;
    }
    return;
}

void set_color_from_hex_code(Color* color, const char* str)
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

void set_color_from_other_color(Color* dst, const Color* src)
{
    dst->r = src->r;
    dst->g = src->g;
    dst->b = src->b;
    dst->a = src->a;
    return;
}

void set_pixel_color(unsigned char* pixel, const Color* color)
{
    pixel[0] = color->r;
    pixel[1] = color->g;
    pixel[2] = color->b;
    pixel[3] = color->a;
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

static Color* create_color(const char* hex_code)
{
    Color* color = malloc(sizeof(Color));

    if (!color)
        return 0;

    set_color_from_hex_code(color, hex_code);
    return color;
}

