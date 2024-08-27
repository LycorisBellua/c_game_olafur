#include "cimmerian.h"
#include "lodepng.h"

int is_digit(int c)
{
    return c >= '0' && c <= '9';
}

char* read_file(char* filepath)
{
    char* ptr;
    long file_length;
    size_t items_read;
    FILE* file;

    ptr = 0;
    file = fopen(filepath, "r");
    if (!file)
    {
        fprintf(stderr, "Error: Couldn't open file from \"%s\"", filepath);
        return 0;
    }

    fseek(file, 0, SEEK_END);
    file_length = ftell(file);
    fseek(file, 0, SEEK_SET);
    if (file_length < 0)
    {
        fclose(file);
        fprintf(stderr, "Error: Could open but not read file from \"%s\"\n", 
            filepath);
        return 0;
    }

    ptr = malloc((file_length + 1) * sizeof(char));
    if (!ptr)
    {
        fprintf(stderr, "Error: Couldn't allocate memory to store content " 
            "from \"%s\"\n", filepath);
        fclose(file);
        return 0;
    }

    items_read = fread(ptr, sizeof(char), file_length, file);
    if (items_read != (size_t)file_length)
    {
        fclose(file);
        free(ptr);
        fprintf(stderr, "Error: Could open but not read file from \"%s\"\n", 
            filepath);
        return 0;
    }
    ptr[file_length] = 0;

    fclose(file);
    return ptr;
}

/* Sprite size must be a power of two */
t_spr* load_sprite(char* png_path, int is_see_through)
{
    t_spr* s;
    t_uivec2 size;
    unsigned int err;
    
    s = malloc(sizeof(t_spr));
    if (!s)
        return 0;
    err = lodepng_decode32_file(&s->buf, &size.x, &size.y, png_path);
    if (err)
    {
        fprintf(stderr, "Error: %s (Lodepng lib - error n°%u)\n", 
            lodepng_error_text(err), err);
        free_sprite(s);
        return 0;
    }
    s->is_see_through = is_see_through;
    s->size.x = size.x;
    s->size.y = size.y;
    return s;
}

t_spr* create_sprite(t_color c, int is_see_through)
{
    t_spr* s;
    int i;

    s = malloc(sizeof(t_spr));
    if (!s)
        return 0;
    s->is_see_through = is_see_through;
    s->size.x = SPR_W;
    s->size.y = SPR_H;
    s->buf = malloc(s->size.x * s->size.y * 4 * sizeof(GLubyte));
    if (!s->buf)
    {
        free_sprite(s);
        return 0;
    }
    i = 0;
    while (i < s->size.x * s->size.y)
        *((t_color*)s->buf + i++) = c;
    return s;
}

void free_sprite(t_spr* s)
{
    free(s->buf);
    free(s);
    return;
}
