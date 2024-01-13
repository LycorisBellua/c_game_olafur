#include "../headers/textures.h"
#include "../headers/windowing.h"
#include "../headers/file_handling.h"
#include "../headers/maths.h"

Texture* textures[NBR_TEXTURES] = {0};

static Texture* create_texture(void);
static void free_texture(Texture** t);

static void set_color_from_hex_string(Color* color, const char* str);
static void darken_color(Color* color, const int percentage);
static void lighten_color(Color* color, const int percentage);

void create_textures(void)
{
    TEXTURE_GAME = create_texture();
    textures[NBR_TEXTURES - 1] = 0;
    return;
}

void set_texture(Texture* t)
{
    int x, y, i;

    glBindTexture(GL_TEXTURE_2D, t->id);

    /* Gradient */
    for (y = 0; y < t->height; ++y)
    {
        for (x = 0; x < t->width; ++x)
        {
            i = (y * t->width + x) * 4;
            t->buffer[i]   = x * 255 / t->width;
            t->buffer[i+1] = y * 255 / t->height;
            t->buffer[i+2] = 255/2;
            t->buffer[i+3] = 255;
        }
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t->width, t->height, 0, 
        GL_RGBA, GL_UNSIGNED_BYTE, t->buffer);
    glBindTexture(GL_TEXTURE_2D, 0);
    return;
}

void free_textures(void)
{
    unsigned int i;
    for (i = 0; i < sizeof(textures)/sizeof(Texture*); ++i)
    {
        if (!textures[i])
            break;
        free_texture(&textures[i]);
    }
    return;
}

static Texture* create_texture(void)
{
    Texture* t = malloc(sizeof(Texture));
    if (!t)
    {
        fprintf(stderr, "ERROR: Couldn't allocate memory for texture.");
        return 0;
    }

    t->id = -1;
    t->width = monitor_size.width;
    t->height = monitor_size.height;

    t->buffer = malloc(t->width * t->height * 4 * sizeof(unsigned char));
    if (!t->buffer)
    {
        fprintf(stderr, "ERROR: Couldn't allocate enough memory for a texture "
            "(= %ld bytes for RGBA texture of a %dx%d resolution).\n", 
            t->width * t->height * 4 * sizeof(unsigned char), 
            t->width, t->height);
        free(t);
        return 0;
    }

    glGenTextures(1, &t->id);
    glBindTexture(GL_TEXTURE_2D, t->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    set_texture(t);

    glBindTexture(GL_TEXTURE_2D, 0);
    return t;
}

static void free_texture(Texture** t)
{
    /*
        Check whether the pointer is null. If it is, it means it doesn't 
        reference any struct, which therefore means that `buffer` is not a 
        pointer to a legal address.
    */
    if (!t)
        return;

    glDeleteTextures(1, &(*t)->id);
    free((*t)->buffer);
    free(*t);

    /* Nullify the reference to prevent a double free */
    *t = 0;
    return;
}

static void set_color_from_hex_string(Color* color, const char* str)
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

static void darken_color(Color* color, const int percentage)
{
    const int perc_to_rgb = 255/100 * percentage;

    color->r = CLAMP(color->r - perc_to_rgb, 0, 255);
    color->g = CLAMP(color->g - perc_to_rgb, 0, 255);
    color->b = CLAMP(color->b - perc_to_rgb, 0, 255);
    return;
}

static void lighten_color(Color* color, const int percentage)
{
    const int perc_to_rgb = 255/100 * percentage;

    color->r = CLAMP(color->r + perc_to_rgb, 0, 255);
    color->g = CLAMP(color->g + perc_to_rgb, 0, 255);
    color->b = CLAMP(color->b + perc_to_rgb, 0, 255);
    return;
}

