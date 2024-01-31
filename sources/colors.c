#include "../headers/colors.h"
#include "../headers/files.h"

GLubyte colors[NBR_COLORS] = {0};

void create_color_palette(void)
{
    /*
       RED AND GREEN
       0 =   0 = 0x00
       1 =  36 = 0x24
       2 =  72 = 0x48
       3 = 109 = 0x6D
       4 = 145 = 0x91
       5 = 182 = 0xB6
       6 = 218 = 0xDA
       7 = 255 = 0xFF

       BLUE
       0 =   0 = 0x00
       1 =  85 = 0x55
       2 = 170 = 0xAA
       3 = 255 = 0xFF
    */

    int r, g, b;
    int i = 0;

    for (r = MAX_RED; r >= 0; --r)
    {
        for (g = MAX_GREEN; g >= 0; --g)
        {
            for (b = MAX_BLUE; b >= 0; --b)
            {
                set_red_channel(&colors[i], r);
                set_green_channel(&colors[i], g);
                set_blue_channel(&colors[i], b);
                ++i;
            }
        }
    }
    return;
}

GLubyte get_color_from_hex_code(const char* str)
{
    /* e.g.: "#097e7b" - "097e7b" */
    const int i = str[0] == '#';
    GLubyte color = 0;
    GLubyte values[3];

    values[0] = hex_char_to_int(str[i+0])*16 + hex_char_to_int(str[i+1]);
    values[1] = hex_char_to_int(str[i+2])*16 + hex_char_to_int(str[i+3]);
    values[2] = hex_char_to_int(str[i+4])*16 + hex_char_to_int(str[i+5]);

    set_red_channel(&color, values[0]/MAX_RED);
    set_green_channel(&color, values[1]/MAX_GREEN);
    set_blue_channel(&color, values[2]/MAX_BLUE);
    return color;
}

GLubyte get_color_from_rgb(const GLubyte r, const GLubyte g, const GLubyte b)
{
    return ((r << 5) & 0xE0) | ((g << 2) & 0x1C) | (b & 0x03);
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

void set_color_from_rgb(GLubyte* color, const GLubyte r, const GLubyte g, 
    const GLubyte b)
{
    *color = ((r & 0x07) << 5) | ((g & 0x07) << 2) | (b & 0x03);
    return;
}

void set_red_channel(GLubyte* color, const GLubyte value)
{
    *color = (*color & 0x1F) | ((value << 5) & 0xE0);
    return;
}
void set_green_channel(GLubyte* color, const GLubyte value)
{
    *color = (*color & 0xE3) | ((value << 2) & 0x1C);
    return;
}
void set_blue_channel(GLubyte* color, const GLubyte value)
{
    *color = (*color & 0xFC) | (value & 0x03);
    return;
}

