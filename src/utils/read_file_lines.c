#include "olafur.h"

static char	*get_whole_file(int fd);
static int	append_line(t_strbuf *b, const char *line);
static int	reserve(t_strbuf *b, size_t size);
static void	remove_comments(char **lines);

char	**read_file_lines(const char *filepath)
{
	int		fd;
	char	*whole;
	char	**lines;

	fd = open(filepath, O_RDONLY);
	if (fd < 0)
	{
		put_error(E_MAP_READ, filepath, 0);
		return (0);
	}
	whole = get_whole_file(fd);
	close(fd);
	lines = split(whole, '\x1C');
	free(whole);
	remove_comments(lines);
	return (lines);
}

/*
	The lines are appended to a buffer that doubles in size when full, so the
	file is read in a time proportional to its size.
*/
static char	*get_whole_file(int fd)
{
	t_strbuf	b;
	char		*line;

	memset(&b, 0, sizeof(t_strbuf));
	line = gnl(fd);
	while (line)
	{
		if (!append_line(&b, line))
		{
			free(line);
			free(b.s);
			return (0);
		}
		free(line);
		line = gnl(fd);
	}
	return (b.s);
}

/*
	Lines are separated by '\x1C'. A line ending with a backslash (before its
	newline, if any) continues on the next one: the backslash and the newline
	are dropped, and no separator is added.
*/
static int	append_line(t_strbuf *b, const char *line)
{
	size_t	len;
	int		is_continued;

	len = strlen(line);
	is_continued = 1;
	if (len >= 1 && line[len - 1] == '\\')
		len -= 1;
	else if (len >= 2 && line[len - 2] == '\\')
		len -= 2;
	else
		is_continued = 0;
	if (!reserve(b, b->len + len + 2))
		return (0);
	memcpy(b->s + b->len, line, len);
	b->len += len;
	if (!is_continued)
		b->s[b->len++] = '\x1C';
	b->s[b->len] = 0;
	return (1);
}

static int	reserve(t_strbuf *b, size_t size)
{
	size_t	new_cap;
	char	*new_s;

	if (size <= b->cap)
		return (1);
	new_cap = b->cap;
	if (!new_cap)
		new_cap = 256;
	while (new_cap < size)
		new_cap *= 2;
	new_s = realloc(b->s, new_cap);
	if (!new_s)
		return (0);
	b->s = new_s;
	b->cap = new_cap;
	return (1);
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
		else
			++i;
	}
	return ;
}
