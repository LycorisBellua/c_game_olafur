#include "olafur.h"

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
