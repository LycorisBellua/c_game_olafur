#include "cimmerian.h"

static int		allocate_array(t_man *man, char **lines);
static int		set_pixel_data(t_man *man, t_png *file, size_t *i_img);
static t_color	*get_pixel_data(t_png *file, int is_shadow);

int	set_image_array(t_man *man, const char *path)
{
	char	**lines;
	t_png	file;
	size_t	i_img;
	int		is_success;
	int		is_parsing_ongoing;

	lines = get_json_content(path);
	if (!lines || !allocate_array(man, lines))
		return (0);
	i_img = 0;
	is_success = 1;
	is_parsing_ongoing = 1;
	bzero(&file, sizeof(t_png));
	while (is_parsing_ongoing && is_success)
	{
		is_parsing_ongoing = set_png_file_obj(&file, lines);
		if (is_parsing_ongoing)
			is_success = set_pixel_data(man, &file, &i_img);
		free_and_reset_png_file_obj(&file);
	}
	free_json_content(lines);
	add_outline_to_font(man->images[1]);
	return (is_success);
}

static int	allocate_array(t_man *man, char **lines)
{
	int	i;
	int	count;

	i = -1;
	count = 0;
	while (lines && lines[++i])
	{
		if (strstr(lines[i], "\"id\":"))
			++count;
	}
	if (!count)
		return (0);
	man->images = calloc(count + 1, sizeof(t_img *));
	i = -1;
	while (man->images && ++i < count)
	{
		man->images[i] = calloc(1, sizeof(t_img));
		if (!man->images[i])
		{
			free_images(man);
			return (0);
		}
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
