#include "cimmerian.h"

#ifndef GNL_BUFFER_SIZE
# define GNL_BUFFER_SIZE 42
#endif

static int	search_newline(char *buffer, int fd, char **line, char **newline);
static char	*add_to_line(char *line, char *added, int free_added);
static char	*get_line(char *src, char *newline_in_src);
static void	shift_buffer(char *buffer, char *newline);

/*
	FEATURES
	----------------------------------------------------------------------------
	- GNL returns a dynamically allocated line read from an fd. A line either 
	ends with '\n' (included) or EOF.
	- If the function is called after EOF or if an error occurred, NULL is 
	returned instead.
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
	if (!search_newline(buffer, fd, &line, &newline_in_buffer))
	{
		free(line);
		return (0);
	}
	if (newline_in_buffer)
		line = add_to_line(line, get_line(buffer, newline_in_buffer), 1);
	shift_buffer(buffer, newline_in_buffer);
	return (line);
}

static int	search_newline(char *buffer, int fd, char **line, char **newline)
{
	ssize_t	read_amount;

	*line = 0;
	*newline = strchr(buffer, '\n');
	while (!*newline)
	{
		*line = add_to_line(*line, buffer, 0);
		read_amount = read(fd, buffer, GNL_BUFFER_SIZE);
		if (read_amount >= 0)
			buffer[read_amount] = 0;
		if (read_amount < 0)
			return (0);
		else if (!read_amount)
		{
			if (!*line[0])
				return (0);
			break ;
		}
		else
			*newline = strchr(buffer, '\n');
	}
	return (1);
}

static char	*add_to_line(char *line, char *added, int free_added)
{
	size_t	len;
	char	*joined;

	if (!line)
	{
		line = malloc(1 * sizeof(char));
		if (line)
			line[0] = 0;
	}
	if (line)
	{
		len = strlen(line) + strlen(added);
		joined = malloc((len + 1) * sizeof(char));
		if (joined)
		{
			joined[0] = 0;
			strncat(joined, line, len + 1);
			strncat(joined, added, len + 1);
			free(line);
			line = joined;
		}
	}
	if (free_added)
		free(added);
	return (line);
}

static char	*get_line(char *src, char *newline_in_src)
{
	char	*dup;
	size_t	len;

	len = newline_in_src - src + 1;
	dup = malloc((len + 1) * sizeof(char));
	if (!dup)
		return (0);
	dup[0] = 0;
	strncat(dup, src, len + 1);
	return (dup);
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
