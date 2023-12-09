#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    int i;
    printf("Debug: %d arguments\n", argc - 1);
    if (argc > 1)
    {
        for (i = 1; i < argc; ++i)
            printf("- %s\n", argv[i]);
    }
    return EXIT_SUCCESS;
}

