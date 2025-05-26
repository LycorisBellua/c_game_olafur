#include "cimmerian.h"

static char	*get_whole_file(int fd);

char	*get_absolute_path(const char *path)
{
	char	*absolute;

	if (!path)
	{
		put_error(0, "get_absolute_path", "null", 0);
		return (0);
	}
	absolute = realpath(path, 0);
	if (!absolute)
		put_error(0, "get_absolute_path", path, 0);
	return (absolute);
}

char	*read_file(const char *filepath)
{
	int		fd;
	char	*content;

	fd = open(filepath, O_RDONLY);
	if (fd < 0)
	{
		printf("Error: Couldn't open file \"%s\" for reading\n", filepath);
		return (0);
	}
	content = get_whole_file(fd);
	close(fd);
	return (content);
}

char	**read_file_lines(const char *filepath)
{
	char	*whole;
	char	**lines;

	whole = read_file(filepath);
	lines = split(whole, '\n');
	free(whole);
	return (lines);
}

static char	*get_whole_file(int fd)
{
	char	*tmp1;
	char	*tmp2;
	char	*tmp3;

	tmp1 = gnl(fd);
	tmp2 = gnl(fd);
	while (tmp2)
	{
		tmp3 = strjoin(tmp1, tmp2);
		free(tmp1);
		free(tmp2);
		tmp1 = tmp3;
		tmp2 = gnl(fd);
	}
	return (tmp1);
}
