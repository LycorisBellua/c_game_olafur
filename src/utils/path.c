#include "olafur.h"

#ifdef _WIN32

char	*realpath(const char *path, char *resolved_path)
{
	char	*res;

	res = _fullpath(resolved_path, path, _MAX_PATH);
	if (res && !_access(res, F_OK))
		return (res);
	free(res);
	return (0);
}
#endif

char	*get_absolute_path(const char *path)
{
	if (!path)
		return (0);
	return (realpath(path, 0));
}

const char	*get_filename(const char *path)
{
	const char	*slash;
	const char	*bslash;
	const char	*name;

	if (!path)
		return (0);
	slash = strrchr(path, '/');
	bslash = strrchr(path, '\\');
	name = slash;
	if (bslash > name)
		name = bslash;
	if (name)
		++name;
	else
		name = path;
	return (name);
}

/*
	At least one character must come before the extension, so a file named only 
	".map" or ".json" doesn't count.
*/
int	has_extension(const char *name, const char *ext)
{
	size_t	len;
	size_t	ext_len;

	if (!name || !ext)
		return (0);
	len = strlen(name);
	ext_len = strlen(ext);
	return (len > ext_len && !strcmp(name + len - ext_len, ext));
}
