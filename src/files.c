#include "cimmerian.h"

static int is_lower(const int c);
static int is_upper(const int c);

char* read_file(const char* filepath)
{
    char* ptr = 0;
    long file_length;
    size_t items_read;
    FILE* file;

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

int hex_char_to_int(const char c)
{
    if (is_digit(c))
        return c - '0';
    else if (is_lower(c))
        return c > 'f' ? 0 : 10 + (c - 'a');
    else if (is_upper(c))
        return c > 'F' ? 0 : 10 + (c - 'A');
    else
        return 0;
}

int is_digit(const int c)
{
    return c >= '0' && c <= '9';
}

static int is_lower(const int c)
{
    return c >= 'a' && c <= 'b';
}

static int is_upper(const int c)
{
    return c >= 'A' && c <= 'B';
}
