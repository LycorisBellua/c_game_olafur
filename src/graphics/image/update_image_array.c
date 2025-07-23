#include "olafur.h"

static int		check_path(t_man *man, const char *path);
static int		allocate_array(t_man *man, char **lines, size_t *i_img);
static int		set_pixel_data(t_man *man, t_png *file, size_t *i_img);
static t_color	*get_pixel_data(t_png *file, int is_shadow);

int	update_image_array(t_man *man, const char *path)
{
	char	**lines;
	t_png	file;
	size_t	i_img;
	int		is_success;
	int		is_parsing_ongoing;

	if (!path || check_path(man, path))
		return (1);
	lines = get_json_content(man, path);
	i_img = 0;
	if (!lines || !allocate_array(man, lines, &i_img))
		return (0);
	is_success = 1;
	is_parsing_ongoing = 1;
	memset(&file, 0, sizeof(t_png));
	while (is_parsing_ongoing && is_success)
	{
		is_parsing_ongoing = set_png_file_obj(&file, lines);
		if (is_parsing_ongoing)
			is_success = set_pixel_data(man, &file, &i_img);
		free_and_reset_png_file_obj(&file);
	}
	free_json_content(lines);
	return (is_success);
}

static int	check_path(t_man *man, const char *path)
{
	size_t	i;
	char	**new_arr;

	if (!man->json_paths)
		return (0);
	i = 0;
	while (man->json_paths[i])
	{
		if (!strcmp(man->json_paths[i], path))
			return (1);
		++i;
	}
	new_arr = calloc(i + 1 + 1, sizeof(char *));
	if (new_arr)
	{
		memcpy(new_arr, man->json_paths, i * sizeof(char *));
		free(man->json_paths);
		man->json_paths = new_arr;
		man->json_paths[i] = strdup(path);
	}
	return (0);
}

static int	allocate_array(t_man *man, char **lines, size_t *i_img)
{
	size_t	i;
	size_t	count;
	size_t	old_count;
	t_img	**new_arr;

	i = 0;
	count = 0;
	while (lines && lines[i])
	{
		if (strstr(lines[i], "\"id\":"))
			++count;
		++i;
	}
	if (!count)
		return (0);
	old_count = count_arr_elems((void **)man->images);
	new_arr = calloc(old_count + count + 1, sizeof(t_img *));
	if (!new_arr)
		return (0);
	memcpy(new_arr, man->images, old_count * sizeof(t_img *));
	free(man->images);
	man->images = new_arr;
	*i_img = old_count;
	i = 0;
	while (i < count)
	{
		man->images[old_count + i] = calloc(1, sizeof(t_img));
		if (!man->images[old_count + i])
		{
			free_images(man);
			return (0);
		}
		++i;
	}
	return (!!man->images);
}

static int	set_pixel_data(t_man *man, t_png *file, size_t *i_img)
{
	size_t	i;

	file->buf = get_pixel_data(file, 0);
	if (!file->buf)
		return (0);
	if (file->path_shadow)
	{
		file->buf_shadow = get_pixel_data(file, 1);
		if (!file->buf_shadow)
			return (0);
	}
	i = 0;
	while (i < file->segment_len)
	{
		file->seg[i].size.x = clamp(file->seg[i].size.x, 0, file->size.x);
		file->seg[i].size.y = clamp(file->seg[i].size.y, 0, file->size.y);
		++i;
	}
	return (create_images_from_file(man, file, i_img));
}

static t_color	*get_pixel_data(t_png *file, int is_shadow)
{
	char	*path;
	t_png	*png;
	t_color	*data;

	path = file->path;
	if (is_shadow)
		path = file->path_shadow;
	png = load_png_from_path(path);
	if (!png)
		return (0);
	set_ivec2(&file->size, png->size.x, png->size.y);
	data = malloc(png->size.x * png->size.y * sizeof(t_color));
	memcpy(data, png->buf, png->size.x * png->size.y * sizeof(t_color));
	free_png(png);
	return (data);
}
