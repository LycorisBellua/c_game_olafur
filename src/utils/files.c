#include "cimmerian.h"

static char	*get_whole_file(int fd);

char	*get_absolute_path(const char *path)
{
	if (!path)
		return (0);
	return (realpath(path, 0));
}

const char	*get_filename(const char *path)
{
	const char	*start;

	if (!path)
		return (0);
	start = strrchr(path, '/');
	if (start)
		++start;
	else
		start = path;
	return (start);
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
	lines = split(whole, '\x1C');
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
		tmp1 = strjoin(tmp3, "\x1C");
		free(tmp3);
		tmp2 = gnl(fd);
	}
	return (tmp1);
}
