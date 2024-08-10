#include "cimmerian.h"

Texture* textures[NBR_TEXTURES] = {0};

static Texture* create_texture(void);
static void free_texture(Texture** t);

void create_textures(void)
{
    TEX_MAIN = create_texture();
    textures[NBR_TEXTURES - 1] = 0;
    return;
}

void use_texture(const Texture* t)
{
    glBindTexture(GL_TEXTURE_2D, t ? t->id : 0);
    return;
}

void clear_drawing(Texture* t)
{
    bzero(t->buffer, t->real_width * t->real_height * 4 * sizeof(GLubyte));
    return;
}

void save_drawing(const Texture* t)
{
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, t->real_width, t->real_height, 0, 
        GL_RGBA, GL_UNSIGNED_BYTE, t->buffer);
    return;
}

void free_textures(void)
{
    unsigned int i;

    use_texture(0);

    i = 0;
    while (i < sizeof(textures)/sizeof(Texture*))
    {
        if (!textures[i])
            break;
        free_texture(&textures[i]);
        ++i;
    }
    return;
}

static Texture* create_texture(void)
{
    Texture* t = malloc(sizeof(Texture));
    long buffer_length;
    if (!t)
    {
        fprintf(stderr, "Error: Couldn't allocate memory for texture");
        return 0;
    }

    t->id = -1;
    t->real_width = res.monitor_width;
    t->real_height = res.monitor_height;
    t->thickness = t->real_width/res.window_width_default;
    t->width = t->real_width / t->thickness;
    t->height = t->real_height / t->thickness;

    buffer_length = t->real_width * t->real_height * 4 * sizeof(GLubyte);
    t->buffer = malloc(buffer_length);
    if (!t->buffer)
    {
        fprintf(stderr, "Error: Couldn't allocate enough memory for a texture "
            "(= %ld bytes for texture of a %dx%d resolution)\n", 
            buffer_length, t->real_width, t->real_height);
        free(t);
        return 0;
    }

    glGenTextures(1, &t->id);
    glBindTexture(GL_TEXTURE_2D, t->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

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
