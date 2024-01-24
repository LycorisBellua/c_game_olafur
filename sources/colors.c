#include "../headers/colors.h"
#include "../headers/files.h"
#include "../headers/maths.h"

GLubyte* colors[NBR_COLORS] = {0};
static int is_little_endian = 0;

static int check_little_endian(void);
static GLubyte* create_color(const char* hex_code);

void create_color_palette(void)
{
    is_little_endian = check_little_endian(); 

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

static int check_little_endian(void)
{
    /*
        The bits in a byte are always ordered the same way: the first digit is 
        the most significant one. Indeed, in a byte, the first bit means 
        "bit_value * 128", whereas the last bit means "bit_value * 1". It's the 
        same in a decimal number, for example "123", where the 1 is the most 
        significant because it represents "1*100=100", and where 3 is the least 
        significant because it represents "3*1=3". 100 is always closer to 123 
        than 3 is.

        This concept of most/least significant is relevant to the concept of 
        endianness. A system is either Little Endian or Big Endian. Endianness 
        is how bytes are ordered in a multi-byte piece of data, not at all 
        about the bits in a given byte, whose order remains as we have seen.

        Big Endian is when what comes first is the MSB (Most Significant Byte), 
        which matches the logic we've seen so far, and Little Endian is when 
        what comes first is the LSB (Least Significant Byte). Most modern 
        machines are Little Endian, which is why we use it as our default.

        To check the endianness, the simplest way is to:
        - declare a variable for a multi-byte piece of data,
        - give it a value of 1, which means that all bytes will be filled with 
        zeros, except for one of them which holds this 1 value, and this byte 
        is the LSB since 1 is the smallest non-null positive value possible,
        - the address starts at the first byte, so as to read what value this 
        first byte holds, cast the address to a char pointer (because a char is 
        one byte long), 
        - dereference it to read the value: 1 means that the LSB came first, 
        and the system is therefore Little Endian, and 0 means that the LSB 
        didn't come first, and therefore the system is Big Endian.
    */

    int n = 1;
    return *(char *)&n;
}

static GLubyte* create_color(const char* hex_code)
{
    GLubyte* color = malloc(4 * sizeof(GLubyte));

    if (!color)
        return 0;

    set_color_from_hex_code(color, hex_code);
    return color;
}

