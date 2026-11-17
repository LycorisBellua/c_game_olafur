#include "olafur.h"

static char	*read_text_file(const char *path);
static int	file_item(t_json *j, void *ctx);
static int	file_member(t_json *j, void *ctx);

int	parse_json_images(t_json *j, const char *path, const char *abs_path)
{
	memset(j, 0, sizeof(t_json));
	j->path = path;
	j->text = read_text_file(abs_path);
	if (!j->text)
		return (0);
	if (!strncmp(j->text, "\xEF\xBB\xBF", 3))
		j->i = 3;
	if (!json_expect(j, '[', E_JSON_ARRAY) || !json_list(j, ']', file_item, j))
		return (0);
	if (json_peek(j))
		return (json_error(j, E_JSON_TRAILING));
	return (1);
}

void	free_json_images(t_json *j)
{
	size_t	i;

	i = 0;
	while (i < j->file_len)
		free_and_reset_png_file_obj(&j->files[i++]);
	free(j->files);
	free(j->text);
	memset(j, 0, sizeof(t_json));
	return ;
}

static char	*read_text_file(const char *path)
{
	struct stat	st;
	char		*text;
	ssize_t		ret;
	size_t		len;
	int			fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (0);
	text = 0;
	if (!fstat(fd, &st) && st.st_size >= 0)
		text = malloc(st.st_size + 1);
	len = 0;
	ret = 1;
	while (text && ret > 0 && len < (size_t)st.st_size)
	{
		ret = read(fd, text + len, st.st_size - len);
		if (ret > 0)
			len += ret;
	}
	close(fd);
	if (text)
		text[len] = 0;
	return (text);
}

static int	file_item(t_json *j, void *ctx)
{
	t_png	*new_arr;

	(void)ctx;
	new_arr = realloc(j->files, (j->file_len + 1) * sizeof(t_png));
	if (!new_arr)
		return (put_error(E_FAIL_MEM, 0, 0));
	j->files = new_arr;
	memset(&j->files[j->file_len], 0, sizeof(t_png));
	++j->file_len;
	if (!json_expect(j, '{', E_JSON_OBJECT))
		return (0);
	return (json_list(j, '}', file_member, &j->files[j->file_len - 1]));
}

static int	file_member(t_json *j, void *ctx)
{
	t_png	*file;
	char	*key;
	int		ok;

	file = ctx;
	ok = json_key(j, &key);
	if (ok && key && !strcmp(key, "path"))
		ok = json_string(j, &file->path);
	else if (ok && key && !strcmp(key, "path_shadow"))
		ok = json_string(j, &file->path_shadow);
	else if (ok && key && !strcmp(key, "segments"))
		ok = json_parse_segments(j, file);
	else if (ok)
		ok = json_unknown_field(j, key);
	free(key);
	return (ok);
}
