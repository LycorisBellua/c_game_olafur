#include "cimmerian.h"

static int		set_pixel_data(t_img *file, int *i_spr);
static GLubyte	*get_pixel_data(t_img *file, int is_shadow);
static t_color	get_pixel(t_img *img, size_t i, int is_shadow);
static void		free_and_reset_img_data(t_img *file);

int	set_sprite_array(char *path)
{
	size_t	i;
	char	**lines;
	t_img	file;
	int		is_parsing_ongoing;

	lines = get_json_content(path);
	if (!lines)
		return (0);

	// parse img file
	is_parsing_ongoing = 1;
	i = 0;
	while (is_parsing_ongoing)
	{
		is_parsing_ongoing = set_img_file_obj(&file, lines, &i);
	}


	// free file lines
	i = 0;
	while (lines[i])
		free(lines[i++]);
	free(lines);
	//
	return (1);

	/*
	int		i_spr;
	int		is_success;
	int		is_parsing_ongoing;
	t_img	file;

	i_spr = 1;
	is_success = 1;
	is_parsing_ongoing = 1;
	bzero(&file, sizeof(t_img));
	while (is_parsing_ongoing && is_success)
	{
		is_parsing_ongoing = set_img_file_obj(&file, fd);
		if (is_parsing_ongoing)
			is_success = set_pixel_data(&file, &i_spr);
		free_and_reset_img_data(&file);
	}
	return (is_success);
	*/
}

static int	set_pixel_data(t_img *file, int *i_spr)
{
	file->buf = get_pixel_data(file, 0);
	if (file->path_shadow)
		file->buf_shadow = get_pixel_data(file, 1);
	if (!file->buf || (file->path_shadow && !file->buf_shadow)
		|| !create_sprites_from_file(file, i_spr))
		return (0);
	return (1);
}

static GLubyte	*get_pixel_data(t_img *file, int is_shadow)
{
	size_t	i;
	size_t	data_len;
	char	*path;
	t_img	*img;
	t_color	*data;

	path = file->path;
	if (is_shadow)
		path = file->path_shadow;
	img = load_image_from_file(path);
	if (!img)
		return (0);
	file->size.x = img->size.x;
	file->size.y = img->size.y;
	data = malloc(img->size.x * img->size.y * sizeof(t_color));
	if (data)
	{
		i = 0;
		data_len = img->size.x * img->size.y;
		while (i < data_len)
		{
			data[i] = get_pixel(img, i, is_shadow);
			++i;
		}
	}
	free_image(img);
	return ((GLubyte *)data);
}

static t_color	get_pixel(t_img *img, size_t i, int is_shadow)
{
	GLubyte	red;
	GLubyte	green;
	GLubyte	blue;
	GLubyte	alpha;

	red = img->buf[i * 4 + 0];
	green = img->buf[i * 4 + 1];
	blue = img->buf[i * 4 + 2];
	alpha = img->buf[i * 4 + 3];
	if (is_shadow)
		alpha = 255 - red;
	return (get_color_rgba(red, green, blue, alpha));
}

static void	free_and_reset_img_data(t_img *file)
{
	int	i;

	free(file->path);
	free(file->path_shadow);
	free(file->buf);
	free(file->buf_shadow);
	i = 0;
	while (i < file->segment_len)
	{
		free(file->seg[i].id);
		free(file->seg[i].cycle);
		++i;
	}
	free(file->seg);
	bzero(file, sizeof(t_img));
	return ;
}
