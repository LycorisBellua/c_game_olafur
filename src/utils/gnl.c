#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef GNL_BUFFER_SIZE
# define GNL_BUFFER_SIZE 42
#endif

static char	*search_newline(char *buffer, int fd, char **line, size_t *len);
static int	append(char **line, size_t *len, const char *added, size_t n);
static void	shift_buffer(char *buffer, char *newline);

/*
	FEATURES
	----------------------------------------------------------------------------
	- GNL returns a dynamically allocated line read from an fd. A line either 
	ends with '\n' (included) or EOF.
	- If the function is called after EOF or if an error occurred (failed read 
	or allocation), NULL is returned instead, and nothing is leaked.
	- GNL may retain some content that has been read but was after a newline. 
	Therefore, after using GNL even once with a specific file, some content 
	might be missing if the same file is directly read with the read system 
	call. In order to avoid that, either finish reading with GNL, move the file 
	offset with lseek, or close and reopen the file.
	- If the fd changes, GNL empties its buffer. This prevents the content of 
	the previous fd from bleeding into the returned line.
	- The buffer also empties if a negative fd is passed as argument. This 
	comes in handy in the case of an opened file getting the same fd as a 
	previously closed file which was read with GNL.
	- It's UB to read from a binary file. Lines will be returned but their 
	content may be funky.
*/

char	*gnl(int fd)
{
	static char	buffer[GNL_BUFFER_SIZE + 1];
	static int	previous_fd;
	char		*line;
	size_t		len;
	char		*newline_in_buffer;

	if (GNL_BUFFER_SIZE < 1)
		return (0);
	if (fd != previous_fd)
	{
		buffer[0] = 0;
		if (fd < 0)
			return (0);
		previous_fd = fd;
	}
	newline_in_buffer = search_newline(buffer, fd, &line, &len);
	if (newline_in_buffer)
		append(&line, &len, buffer, newline_in_buffer - buffer + 1);
	else if (!len)
	{
		free(line);
		return (0);
	}
	shift_buffer(buffer, newline_in_buffer);
	return (line);
}

static char	*search_newline(char *buffer, int fd, char **line, size_t *len)
{
	ssize_t	read_amount;
	char	*newline;

	*line = 0;
	*len = 0;
	newline = strchr(buffer, '\n');
	while (!newline)
	{
		if (!append(line, len, buffer, strlen(buffer)))
			return (0);
		buffer[0] = 0;
		read_amount = read(fd, buffer, GNL_BUFFER_SIZE);
		if (read_amount < 0)
		{
			free(*line);
			*line = 0;
			*len = 0;
			return (0);
		}
		buffer[read_amount] = 0;
		if (!read_amount)
			return (0);
		newline = strchr(buffer, '\n');
	}
	return (newline);
}

/*
	The line's capacity is always the smallest of 64, 128, 256... that fits it, 
	so it can be worked out from the length alone. It doubles whenever the line 
	fills up, so a long line is copied a handful of times instead of once per 
	buffer read.
*/
static int	append(char **line, size_t *len, const char *added, size_t n)
{
	char	*grown;
	size_t	cap;

	cap = 64;
	while (cap < *len + 1)
		cap *= 2;
	if (!*line || *len + n + 1 > cap)
	{
		while (cap < *len + n + 1)
			cap *= 2;
		grown = realloc(*line, cap);
		if (!grown)
		{
			free(*line);
			*line = 0;
			*len = 0;
			return (0);
		}
		*line = grown;
	}
	memcpy(*line + *len, added, n);
	*len += n;
	(*line)[*len] = 0;
	return (1);
}

static void	shift_buffer(char *buffer, char *newline)
{
	size_t	i;

	i = 0;
	if (newline)
	{
		while (newline[i + 1])
		{
			buffer[i] = newline[i + 1];
			++i;
		}
	}
	buffer[i] = 0;
	return ;
}
