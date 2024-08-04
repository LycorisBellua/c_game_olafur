#include "cimmerian.h"

void execute_cli_options(int argc, char** argv)
{
    int i;
    char* str;
    char* token;
    int len;

    if (argc < 2)
        return;

    for (i = 1; i < argc; ++i)
    {
        len = strlen(argv[i]) + 1;
        str = malloc(len * sizeof(char));
        strcpy(str, argv[i]);
        str[len - 1] = 0;

        token = strtok(str, "-=");
        if (token && !strcmp(token, "vsync"))
        {
            token = strtok(NULL, "-=");
            if (token)
            {
                if (!strcmp(token, "on"))
                    enable_vsync(1);
                else if (!strcmp(token, "off"))
                    enable_vsync(0);
            }
        }

        free(str);
    }
    return;
}

void enable_vsync(const int enable)
{
    /*
        Vsync caps the FPS to the monitor's refresh rate.
        1 to enable, 0 to disable.
    */
    glfwSwapInterval(enable != 0);
    return;
}
