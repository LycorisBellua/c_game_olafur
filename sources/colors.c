#include "../headers/colors.h"
#include "../headers/files.h"
#include "../headers/maths.h"

GLuint* colors[NBR_COLORS] = {0};

GLuint (*get_color_from_rgba)(const GLubyte r, const GLubyte g, 
    const GLubyte b, const GLubyte a);
void (*set_color_from_int)(GLuint* color, const GLuint value);
GLubyte (*get_red_channel)(const GLuint color);
GLubyte (*get_green_channel)(const GLuint color);
GLubyte (*get_blue_channel)(const GLuint color);
GLubyte (*get_alpha_channel)(const GLuint color);
void (*set_red_channel)(GLuint* color, const GLubyte value);
void (*set_green_channel)(GLuint* color, const GLubyte value);
void (*set_blue_channel)(GLuint* color, const GLubyte value);
void (*set_alpha_channel)(GLuint* color, const GLubyte value);

static void set_function_pointers(void);
static GLuint* create_color(const char* hex_code);

static GLuint be_get_color_from_rgba(const GLubyte r, const GLubyte g, 
    const GLubyte b, const GLubyte a);
static void be_set_color_from_int(GLuint* color, const GLuint value);
static GLuint le_get_color_from_rgba(const GLubyte r, const GLubyte g, 
    const GLubyte b, const GLubyte a);
static void le_set_color_from_int(GLuint* color, const GLuint value);
static GLubyte get_first_byte(const GLuint color);
static GLubyte get_second_byte(const GLuint color);
static GLubyte get_third_byte(const GLuint color);
static GLubyte get_fourth_byte(const GLuint color);
static void set_first_byte(GLuint* color, const GLubyte value);
static void set_second_byte(GLuint* color, const GLubyte value);
static void set_third_byte(GLuint* color, const GLubyte value);
static void set_fourth_byte(GLuint* color, const GLubyte value);

void create_color_palette(void)
{
    set_function_pointers();

    colors[COLOR_WHITE] = create_color("#FFFFFF");
    colors[COLOR_RED] = create_color("#FF0000");
    colors[COLOR_GREEN] = create_color("#00FF00");
    colors[COLOR_BLUE] = create_color("#0000FF");
    colors[NBR_COLORS - 1] = 0;
    return;
}

void set_color_from_hex_code(GLuint* color, const char* str)
{
    /* e.g.: "#097e7bff" - "#097e7b" - "097e7b" */
    int i = str[0] == '#';
    GLubyte values[4];

    values[0] = hex_char_to_int(str[i+0])*16 + hex_char_to_int(str[i+1]);
    values[1] = hex_char_to_int(str[i+2])*16 + hex_char_to_int(str[i+3]);
    values[2] = hex_char_to_int(str[i+4])*16 + hex_char_to_int(str[i+5]);
    values[3] = !str[i+6] ? 255 
        : hex_char_to_int(str[i+6])*16 + hex_char_to_int(str[i+7]);

    set_red_channel(color, values[0]);
    set_green_channel(color, values[1]);
    set_blue_channel(color, values[2]);
    set_alpha_channel(color, values[3]);
    return;
}

void free_color_palette(void)
{
    unsigned int i;
    for (i = 0; i < sizeof(colors)/sizeof(GLuint*); ++i)
    {
        if (!colors[i])
            break;

        free(colors[i]);
        /* Nullify the reference to prevent a double free */
        colors[i] = 0;
    }
    return;
}

static void set_function_pointers(void)
{
    const int n = 1;
    const int is_big_endian = *(char *)&n == 0;

    if (is_big_endian)
    {
        get_color_from_rgba = &be_get_color_from_rgba;
        set_color_from_int = &be_set_color_from_int;
        get_red_channel = &get_first_byte;
        get_green_channel = &get_second_byte;
        get_blue_channel = &get_third_byte;
        get_alpha_channel = &get_fourth_byte;
        set_red_channel = &set_first_byte;
        set_green_channel = &set_second_byte;
        set_blue_channel = &set_third_byte;
        set_alpha_channel = &set_fourth_byte;
    }
    else
    {
        get_color_from_rgba = &le_get_color_from_rgba;
        set_color_from_int = &le_set_color_from_int;
        get_red_channel = &get_fourth_byte;
        get_green_channel = &get_third_byte;
        get_blue_channel = &get_second_byte;
        get_alpha_channel = &get_first_byte;
        set_red_channel = &set_fourth_byte;
        set_green_channel = &set_third_byte;
        set_blue_channel = &set_second_byte;
        set_alpha_channel = &set_first_byte;
    }
    return;
}

static GLuint* create_color(const char* hex_code)
{
    GLuint* color = malloc(sizeof(GLuint));

    if (!color)
        return 0;

    set_color_from_hex_code(color, hex_code);
    return color;
}

static GLuint be_get_color_from_rgba(const GLubyte r, const GLubyte g, const GLubyte b, 
    const GLubyte a)
{
    return (r << 24) | (g << 16) | (b << 8) | a;
}
static GLuint le_get_color_from_rgba(const GLubyte r, const GLubyte g, const GLubyte b, 
    const GLubyte a)
{
    return (a << 24) | (b << 16) | (g << 8) | r;
}

static void be_set_color_from_int(GLuint* color, const GLuint value)
{
    *color = value;
    return;
}
static void le_set_color_from_int(GLuint* color, const GLuint value)
{
    set_red_channel(color, get_red_channel(value));
    set_green_channel(color, get_green_channel(value));
    set_blue_channel(color, get_blue_channel(value));
    set_alpha_channel(color, get_alpha_channel(value));
    return;
}

static GLubyte get_first_byte(const GLuint color)
{
    return color >> 24;
}
static GLubyte get_second_byte(const GLuint color)
{
    return (color >> 16) & 0xFF;
}
static GLubyte get_third_byte(const GLuint color)
{
    return (color >> 8) & 0xFF;
}
static GLubyte get_fourth_byte(const GLuint color)
{
    return color & 0xFF;
}

static void set_first_byte(GLuint* color, const GLubyte value)
{
    *color = (*color & 0x00FFFFFF) | (value << 24);
    return;
}
static void set_second_byte(GLuint* color, const GLubyte value)
{
    *color = (*color & 0xFF00FFFF) | (value << 16);
    return;
}
static void set_third_byte(GLuint* color, const GLubyte value)
{
    *color = (*color & 0xFFFF00FF) | (value << 8);
    return;
}
static void set_fourth_byte(GLuint* color, const GLubyte value)
{
    *color = (*color & 0xFFFFFF00) | value;
    return;
}

