#include "cimmerian.h"

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
