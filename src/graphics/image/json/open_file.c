#include "cimmerian.h"

static char	**get_line_arr(const char *filepath);
static char	*remove_leading_whitespaces(char *s);

char	**get_json_content(t_man *man, const char *filepath)
{
	size_t	i;
	int		fd;
	char	*line;
	char	**lines;

	(void)man;
	lines = get_line_arr(filepath);
	if (!lines)
		return (0);
	fd = open(filepath, O_RDONLY);
	if (fd < 0)
	{
		free(lines);
		return (0);
	}
	i = 0;
	line = gnl(fd);
	while (line)
	{
		line[strlen(line) - 1] = 0;
		lines[i++] = remove_leading_whitespaces(line);
		line = gnl(fd);
	}
	close(fd);
	return (lines);
}

void	free_json_content(char **content)
{
	size_t	i;

	if (!content)
		return ;
	i = 0;
	while (content[i])
	{
		free(content[i]);
		++i;
	}
	free(content);
	return ;
}

static char	**get_line_arr(const char *filepath)
{
	int		fd;
	size_t	line_count;
	char	*line;

	fd = open(filepath, O_RDONLY);
	if (fd < 0)
		return (0);
	line_count = 0;
	line = gnl(fd);
	while (line)
	{
		++line_count;
		free(line);
		line = gnl(fd);
	}
	close(fd);
	if (!line_count)
		return (0);
	return (calloc(line_count + 1, sizeof(char *)));
}

static char	*remove_leading_whitespaces(char *s)
{
	size_t	whitespaces_len;

	if (!s)
		return (0);
	whitespaces_len = 0;
	while (isspace(s[whitespaces_len]))
		++whitespaces_len;
	memmove(s, s + whitespaces_len, strlen(s + whitespaces_len) + 1);
	return (s);
}
