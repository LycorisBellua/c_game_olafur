#include "../headers/file_handling.h"

char* read_file(const char* filepath)
{
    char *ptr = 0;
    long file_length;
    size_t items_read;
    FILE* file;

    file = fopen(filepath, "r");
    if (!file)
    {
        fprintf(stderr, "ERROR: Couldn't open file from \"%s\".", filepath);
        return 0;
    }

    fseek(file, 0, SEEK_END);
    file_length = ftell(file);
    fseek(file, 0, SEEK_SET);
    if (file_length < 0)
    {
        fclose(file);
        fprintf(stderr, "ERROR: Could open but not read file from \"%s\".\n", 
            filepath);
        return 0;
    }

    ptr = malloc((file_length + 1) * sizeof(char));
    if (!ptr)
    {
        fprintf(stderr, "ERROR: Couldn't allocate memory to store content " 
            "from \"%s\".\n", filepath);
        fclose(file);
        return 0;
    }

    items_read = fread(ptr, sizeof(char), file_length, file);
    if (items_read != (size_t)file_length)
    {
        fclose(file);
        free(ptr);
        fprintf(stderr, "ERROR: Could open but not read file from \"%s\".\n", 
            filepath);
        return 0;
    }
    ptr[file_length] = 0;

    fclose(file);
    return ptr;
}

int char_is_digit(const char c)
{
    return c >= '0' && c <= '9';
}

int char_is_lowercase(const char c)
{
    return c >= 'a' && c <= 'z';
}

int char_is_uppercase(const char c)
{
    return c >= 'A' && c <= 'Z';
}

int hex_char_to_int(const char c)
{
    if (char_is_digit(c))
        return c - '0';
    else if (char_is_lowercase(c))
        return c > 'f' ? 0 : 10 + (c - 'a');
    else if (char_is_uppercase(c))
        return c > 'F' ? 0 : 10 + (c - 'A');
    else
        return 0;
}

