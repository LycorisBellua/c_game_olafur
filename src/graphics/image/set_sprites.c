#include "cimmerian.h"

static int		set_pixel_data(t_img *file, size_t *i_spr);
static t_color	*get_pixel_data(t_img *file, int is_shadow);
static t_color	get_pixel(t_img *img, size_t i, int is_shadow);

int	set_sprite_array(char *path)
{
	char	**lines;
	t_img	file;
	size_t	i_spr;
	int		is_success;
	int		is_parsing_ongoing;

	lines = get_json_content(path);
	if (!lines)
		return (0);
	i_spr = 0;
	is_success = 1;
	is_parsing_ongoing = 1;
	bzero(&file, sizeof(t_img));
	while (is_parsing_ongoing && is_success)
	{
		is_parsing_ongoing = set_img_file_obj(&file, lines);
		if (is_parsing_ongoing)
			is_success = set_pixel_data(&file, &i_spr);
		free_and_reset_img_file_obj(&file);
	}
	free_json_content(lines);
	return (is_success);
}

static int	set_pixel_data(t_img *file, size_t *i_spr)
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
	return (create_sprites_from_file(file, i_spr));
}

static t_color	*get_pixel_data(t_img *file, int is_shadow)
{
	int		i;
	char	*path;
	t_img	*img;
	t_color	*data;

	path = file->path;
	if (is_shadow)
		path = file->path_shadow;
	img = load_image_from_file(path);
	if (!img)
		return (0);
	set_ivec2(&file->size, img->size.x, img->size.y);
	data = calloc(img->size.x * img->size.y, sizeof(t_color));
	if (data)
	{
		i = 0;
		while (i < img->size.x * img->size.y)
		{
			data[i] = get_pixel(img, i, is_shadow);
			++i;
		}
	}
	free_image(img);
	return (data);
}

static t_color	get_pixel(t_img *img, size_t i, int is_shadow)
{
	GLubyte	red;
	GLubyte	green;
	GLubyte	blue;
	GLubyte	alpha;

	red = ((GLubyte *)img->buf)[i * 4 + 0];
	green = ((GLubyte *)img->buf)[i * 4 + 1];
	blue = ((GLubyte *)img->buf)[i * 4 + 2];
	alpha = ((GLubyte *)img->buf)[i * 4 + 3];
	if (is_shadow)
		alpha = 255 - red;
	return (get_color_rgba(red, green, blue, alpha));
}
