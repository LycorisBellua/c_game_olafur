#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

typedef struct s_strlist
{
	char	**items;
	size_t	count;
	size_t	cap;
}	t_strlist;

static void	strlist_init(t_strlist *list);
static int	scan_dir(const char *dir_path, t_strlist *files, t_strlist *subdirs);
static void scan_dir_entry(const char *dir_path, struct dirent *entry,
	            t_strlist *files, t_strlist *subdirs);
static int	has_map_extension(const char *name);
static void	strlist_push(t_strlist *list, const char *name);
static char	*pick_first_file(const char *dir_path, t_strlist *files);
static void	sort_strings(char **arr, size_t n);
static char	*search_subdirs(const char *dir_path, t_strlist *subdirs);
static void	strlist_free(t_strlist *list);

char	*find_first_map_path(const char *dir_path)
{
	t_strlist	files;
	t_strlist	subdirs;
	char		*result;

	strlist_init(&files);
	strlist_init(&subdirs);
	if (!scan_dir(dir_path, &files, &subdirs))
		return (0);
	result = 0;
	if (files.count > 0)
		result = pick_first_file(dir_path, &files);
	else if (subdirs.count > 0)
		result = search_subdirs(dir_path, &subdirs);
	strlist_free(&files);
	strlist_free(&subdirs);
	return (result);
}

static void	strlist_init(t_strlist *list)
{
	list->items = 0;
	list->count = 0;
	list->cap = 0;
}

static int	scan_dir(const char *dir_path, t_strlist *files, t_strlist *subdirs)
{
	DIR				*dir;
	struct dirent	*entry;

	dir = opendir(dir_path);
	if (!dir)
		return (0);
	entry = readdir(dir);
	while (entry)
	{
		if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
			scan_dir_entry(dir_path, entry, files, subdirs);
		entry = readdir(dir);
	}
	closedir(dir);
	return (1);
}

static void	scan_dir_entry(const char *dir_path, struct dirent *entry,
		t_strlist *files, t_strlist *subdirs)
{
	char			full_path[4096];
	struct stat		st;
	int				n;

	n = snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);
	if (n < 0 || (size_t)n >= sizeof(full_path))
		return ;
	if (stat(full_path, &st) < 0)
		return ;
	if (S_ISREG(st.st_mode) && has_map_extension(entry->d_name))
		strlist_push(files, entry->d_name);
	else if (S_ISDIR(st.st_mode))
		strlist_push(subdirs, entry->d_name);
}

static int	has_map_extension(const char *name)
{
	size_t	len;
	size_t	ext_len;

	len = strlen(name);
	ext_len = 4;
	if (len < ext_len)
		return (0);
	return (!strcmp(name + len - ext_len, ".map"));
}

static void	strlist_push(t_strlist *list, const char *name)
{
	if (list->count == list->cap)
	{
		list->cap = !list->cap ? 8 : list->cap * 2;
		list->items = realloc(list->items, list->cap * sizeof(char *));
	}
	list->items[list->count++] = strdup(name);
}

static char	*pick_first_file(const char *dir_path, t_strlist *files)
{
	char	full_path[4096];

	sort_strings(files->items, files->count);
	snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, files->items[0]);
	return (strdup(full_path));
}

static void	sort_strings(char **arr, size_t n)
{
	size_t	i;
	size_t	j;
	char	*key;

	i = 1;
	while (i < n)
	{
		key = arr[i];
		j = i;
		while (j > 0 && strcmp(arr[j - 1], key) > 0)
		{
			arr[j] = arr[j - 1];
			--j;
		}
		arr[j] = key;
		++i;
	}
}

static char	*search_subdirs(const char *dir_path, t_strlist *subdirs)
{
	char	full_path[4096];
	char	*result;
	size_t	i;

	sort_strings(subdirs->items, subdirs->count);
	result = 0;
	i = 0;
	while (i < subdirs->count && !result)
	{
		snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, subdirs->items[i]);
		result = find_first_map_path(full_path);
		++i;
	}
	return (result);
}

static void	strlist_free(t_strlist *list)
{
	size_t	i;

	i = 0;
	while (i < list->count)
	{
		free(list->items[i]);
		++i;
	}
	free(list->items);
}
