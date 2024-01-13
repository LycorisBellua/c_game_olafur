#include "../headers/textures.h"
#include "../headers/windowing.h"

Texture* textures[NBR_TEXTURES] = {0};

static Texture* create_texture(void);
static void free_texture(Texture** t);

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
            t->buffer[i+2] = t->buffer[i]/2 + t->buffer[i+1]/2;
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

