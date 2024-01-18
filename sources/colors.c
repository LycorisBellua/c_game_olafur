#include "../headers/colors.h"
#include "../headers/file_handling.h"
#include "../headers/maths.h"

GLubyte* color_default = 0;
GLubyte* colors[NBR_COLORS] = {0};

static GLubyte* create_color(const char* hex_code);

void create_color_palette(void)
{
    colors[COLOR_BLACK] = create_color("#000000");
    colors[COLOR_WHITE] = create_color("#FFFFFF");
    colors[COLOR_RED] = create_color("#FF0000");
    colors[COLOR_GREEN] = create_color("#00FF00");
    colors[COLOR_BLUE] = create_color("#0000FF");
    colors[NBR_COLORS - 1] = 0;

    color_default = colors[COLOR_WHITE];
    return;
}

void free_color_palette(void)
{
    unsigned int i;
    for (i = 0; i < sizeof(colors)/sizeof(GLubyte*); ++i)
    {
        if (!colors[i])
            break;

        free(colors[i]);
        /* Nullify the reference to prevent a double free */
        colors[i] = 0;
    }
    return;
}

void set_color_from_hex_code(GLubyte* color, const char* str)
{
    /* e.g.: "#097e7bff" - "#097e7b" - "097e7b" */
    int i = str[0] == '#';

    color[0] = hex_char_to_int(str[i+0])*16 + hex_char_to_int(str[i+1]);
    color[1] = hex_char_to_int(str[i+2])*16 + hex_char_to_int(str[i+3]);
    color[2] = hex_char_to_int(str[i+4])*16 + hex_char_to_int(str[i+5]);

    color[3] = !str[i+6] ? 255 
        : hex_char_to_int(str[i+6])*16 + hex_char_to_int(str[i+7]);
    return;
}

void set_color_from_other_color(GLubyte* dst, const GLubyte* src)
{
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = src[3];
    return;
}

void darken_color(GLubyte* color, const int percentage)
{
    const GLubyte perc_to_rgb = 255/100 * percentage;

    color[0] = CLAMP(color[0] - perc_to_rgb, 0, 255);
    color[1] = CLAMP(color[1] - perc_to_rgb, 0, 255);
    color[2] = CLAMP(color[2] - perc_to_rgb, 0, 255);
    return;
}

void lighten_color(GLubyte* color, const int percentage)
{
    const GLubyte perc_to_rgb = 255/100 * percentage;

    color[0] = CLAMP(color[0] + perc_to_rgb, 0, 255);
    color[1] = CLAMP(color[1] + perc_to_rgb, 0, 255);
    color[2] = CLAMP(color[2] + perc_to_rgb, 0, 255);
    return;
}

static GLubyte* create_color(const char* hex_code)
{
    GLubyte* color = malloc(4 * sizeof(GLubyte));

    if (!color)
        return 0;

    set_color_from_hex_code(color, hex_code);
    return color;
}

