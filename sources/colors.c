#include "../headers/colors.h"
#include "../headers/files.h"

GLubyte colors[NBR_COLORS] = {0};

void create_color_palette(void)
{
    colors[COLOR_WHITE] = get_color_from_hex_code("#FFFFFF");
    colors[COLOR_RED] = get_color_from_hex_code("#FF0000");
    colors[COLOR_GREEN] = get_color_from_hex_code("#00FF00");
    colors[COLOR_BLUE] = get_color_from_hex_code("#0000FF");
    colors[NBR_COLORS - 1] = 0;
    return;
}

GLubyte get_color_from_hex_code(const char* str)
{
    /* e.g.: "#097e7b" - "097e7b" */
    GLubyte color = 0;
    int i = str[0] == '#';
    GLubyte values[4];

    values[0] = hex_char_to_int(str[i+0])*16 + hex_char_to_int(str[i+1]);
    values[1] = hex_char_to_int(str[i+2])*16 + hex_char_to_int(str[i+3]);
    values[2] = hex_char_to_int(str[i+4])*16 + hex_char_to_int(str[i+5]);

    set_red_channel(&color, values[0]);
    set_green_channel(&color, values[1]);
    set_blue_channel(&color, values[2]);
    return color;
}

GLubyte get_color_from_rgb(GLubyte r_3bits, GLubyte g_3bits, GLubyte b_2bits)
{
    if (r_3bits > 7)
        r_3bits = 7;
    if (g_3bits > 7)
        g_3bits = 7;
    if (b_2bits > 3)
        b_2bits = 3;
    return ((r_3bits << 5) & 0xE0) | ((g_3bits << 2) & 0x1C) | (b_2bits & 0x03);
}

GLubyte get_red_channel(const GLubyte color)
{
    return (color & 0xE0) >> 5;
}
GLubyte get_green_channel(const GLubyte color)
{
    return (color & 0x1C) >> 2;
}
GLubyte get_blue_channel(const GLubyte color)
{
    return color & 0x03;
}

void set_red_channel(GLubyte* color, GLubyte value_3bits)
{
    if (value_3bits > 7)
        value_3bits = 7;
    *color = (*color & 0x1F) | ((value_3bits << 5) & 0xE0);
    return;
}
void set_green_channel(GLubyte* color, GLubyte value_3bits)
{
    if (value_3bits > 7)
        value_3bits = 7;
    *color = (*color & 0xE3) | ((value_3bits << 2) & 0x1C);
    return;
}
void set_blue_channel(GLubyte* color, GLubyte value_2bits)
{
    if (value_2bits > 3)
        value_2bits = 3;
    *color = (*color & 0xFC) | (value_2bits & 0x03);
    return;
}

