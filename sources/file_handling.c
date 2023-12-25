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

