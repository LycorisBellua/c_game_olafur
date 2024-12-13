#include "cimmerian.h"

static char *get_whole_file(const char *filepath, int fd);
static void join_lines(const char *buf, char **tmp);

char *read_file(const char *filepath)
{
    int fd;
    char *content;

    fd = open(filepath, O_RDONLY);
    if (fd < 0)
    {
        dprintf(STDERR_FILENO, "Error: Couldn't open file \"%s\" for reading\n",
            filepath);
        return 0;
    }
    content = get_whole_file(filepath, fd);
    close(fd);
    return content;
}

static char *get_whole_file(const char *filepath, int fd)
{
    char buf[1024];
    ssize_t len;
    char *tmp;

    tmp = 0;
    len = read(fd, buf, sizeof(buf) - 1);
    while (len > 0)
    {
        buf[len] = 0;
        join_lines(buf, &tmp);
        len = read(fd, buf, sizeof(buf) - 1);
    }
    if (len < 0)
    {
        dprintf(STDERR_FILENO, "Error: Couldn't read file \"%s\"\n", filepath);
        free(tmp);
        tmp = 0;
    }
    return tmp;
}

static void join_lines(const char *buf, char **tmp)
{
    char *tmp2;
    char *tmp3;

    if (!*tmp)
        *tmp = strdup(buf);
    else
    {
        tmp2 = strdup(buf);
        tmp3 = strjoin(*tmp, tmp2);
        free(*tmp);
        free(tmp2);
        *tmp = tmp3;
    }
    return;
}
