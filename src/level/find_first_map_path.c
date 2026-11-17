#include "olafur.h"

static int	scan_dir(const char *dir_path, t_strlist *files,
				t_strlist *subdirs);
static int	scan_dir_entry(const char *dir_path, const char *name,
				t_strlist *files, t_strlist *subdirs);
static char	*search_subdirs(const char *dir_path, t_strlist *subdirs);
static char	*join_path(const char *dir_path, const char *name);

/*
	The first *.map file of the folder, in alphabetical order. If there is 
	none, the subfolders are searched the same way, in alphabetical order.
*/
char	*find_first_map_path(const char *dir_path)
{
	t_strlist	files;
	t_strlist	subdirs;
	char		*result;

	memset(&files, 0, sizeof(t_strlist));
	memset(&subdirs, 0, sizeof(t_strlist));
	result = 0;
	if (scan_dir(dir_path, &files, &subdirs))
	{
		sort_strlist(&files);
		sort_strlist(&subdirs);
		if (files.count > 0)
			result = join_path(dir_path, files.items[0]);
		else
			result = search_subdirs(dir_path, &subdirs);
	}
	free_strlist(&files);
	free_strlist(&subdirs);
	return (result);
}

static int	scan_dir(const char *dir_path, t_strlist *files,
	t_strlist *subdirs)
{
	DIR				*dir;
	struct dirent	*entry;
	int				is_success;

	dir = opendir(dir_path);
	if (!dir)
		return (0);
	is_success = 1;
	entry = readdir(dir);
	while (entry && is_success)
	{
		if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
			is_success = scan_dir_entry(dir_path, entry->d_name, files,
				subdirs);
		entry = readdir(dir);
	}
	closedir(dir);
	return (is_success);
}

static int	scan_dir_entry(const char *dir_path, const char *name,
	t_strlist *files, t_strlist *subdirs)
{
	char		*full_path;
	struct stat	st;
	int			is_success;

	full_path = join_path(dir_path, name);
	if (!full_path)
		return (0);
	is_success = 1;
	if (!stat(full_path, &st))
	{
		if (S_ISREG(st.st_mode) && has_extension(name, ".map"))
			is_success = push_strlist(files, name);
		else if (S_ISDIR(st.st_mode))
			is_success = push_strlist(subdirs, name);
	}
	free(full_path);
	return (is_success);
}

static char	*search_subdirs(const char *dir_path, t_strlist *subdirs)
{
	char	*full_path;
	char	*result;
	size_t	i;

	result = 0;
	i = 0;
	while (i < subdirs->count && !result)
	{
		full_path = join_path(dir_path, subdirs->items[i]);
		if (!full_path)
			return (0);
		result = find_first_map_path(full_path);
		free(full_path);
		++i;
	}
	return (result);
}

static char	*join_path(const char *dir_path, const char *name)
{
	char	*tmp;
	char	*path;

	tmp = strjoin(dir_path, "/");
	if (!tmp)
		return (0);
	path = strjoin(tmp, name);
	free(tmp);
	return (path);
}
