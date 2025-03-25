#include "cimmerian.h"

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
	if (!lines)
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
	return (is_success);
}

static int	set_pixel_data(t_man *man, t_png *file, size_t *i_img)
{
	file->buf = get_pixel_data(file, 0);
	if (!file->buf)
		return (0);
	if (file->path_shadow)
	{
		file->buf_shadow = get_pixel_data(file, 1);
		if (!file->buf_shadow)
			return (0);
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
