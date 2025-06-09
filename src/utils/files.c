#include "cimmerian.h"

static char	*get_whole_file(int fd);
static char	*add_separator(char *line);
static void	remove_comments(char **lines);

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
	remove_comments(lines);
	return (lines);
}

static char	*get_whole_file(int fd)
{
	char	*tmp1;
	char	*tmp2;
	char	*tmp3;

	tmp1 = add_separator(gnl(fd));
	tmp2 = gnl(fd);
	while (tmp2)
	{
		tmp3 = strjoin(tmp1, tmp2);
		free(tmp1);
		free(tmp2);
		tmp1 = add_separator(tmp3);
		tmp2 = gnl(fd);
	}
	return (tmp1);
}

static char	*add_separator(char *line)
{
	size_t	len;
	char	*slash;
	char	*tmp;

	slash = 0;
	if (line)
	{
		len = strlen(line);
		if (!len)
			return (0);
		if (line[len - 1] == '\\')
			slash = line + (len - 1);
		else if (line[len - 2] == '\\')
			slash = line + (len - 2);
		if (slash)
			*slash = 0;
	}
	if (!slash)
	{
		tmp = strjoin(line, "\x1C");
		free(line);
		return (tmp);
	}
	return (line);
}

static void	remove_comments(char **lines)
{
	int		i;
	char	*comment;

	if (!lines)
		return ;
	i = 0;
	while (lines[i])
	{
		comment = strchr(lines[i], '#');
		if (comment)
			*comment = 0;
		if (comment == lines[i])
			remove_arr_elems((void **)lines, i, i, free);
		++i;
	}
	return ;
}
