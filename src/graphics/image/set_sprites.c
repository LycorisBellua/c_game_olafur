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
	bzero(&file, sizeof(t_img));

	// parse img file
	i = 0;
	is_parsing_ongoing = 1;
	while (is_parsing_ongoing)
	{
		is_parsing_ongoing = set_img_file_obj(&file, lines, &i);

		printf("----------\n");
		printf("path = `%s`\n", file.path);
		printf("path_shadow = `%s`\n", file.path_shadow);
		printf("size = (%d, %d)\n", file.size.x, file.size.y);
		printf("[not yet] buf = %p\n", file.buf);
		printf("[not yet] buf_shadow = %p\n", file.buf_shadow);
		printf("segment_len = %d\n", file.segment_len);
		printf("seg = %p\n", (void *)file.seg);
		int j = 0;
		while (file.seg && j < file.segment_len)
		{
			printf("seg[%d].id = `%s`\n", j, file.seg[j].id);
			printf("seg[%d].size = (%d, %d)\n",
				j, file.seg[j].size.x, file.seg[j].size.y);
			printf("seg[%d].shadow_offset = (%d, %d)\n",
				j, file.seg[j].shadow_offset.x, file.seg[j].shadow_offset.y);
			printf("seg[%d].still_frame = %d\n", j, file.seg[j].still_frame);
			printf("seg[%d].cycle_len = %d\n", j, file.seg[j].still_frame);
			printf("seg[%d].cycle_time_in_ms = %ld\n",
				j, file.seg[j].cycle_time_in_ms);
			printf("seg[%d].cycle = %p\n", j, (void *)file.seg[j].cycle);
			int k = 0;
			while (file.seg[j].cycle && k < file.seg[j].cycle_len)
			{
				printf("seg[%d].cycle[%d] = (%d, %d)\n",
					j, k, file.seg[j].cycle[k].x, file.seg[j].cycle[k].y);
				++k;
			}
			++j;
		}
	}

	/*
		typedef struct s_spr
		{
			char	*id;
			t_ivec2	size;
			t_ivec2	shadow_offset;
			int		still_frame;
			int		cycle_len;
			int		cycle_index;
			long	elapsed_time_in_ms;
			long	cycle_time_in_ms;
			t_color	**cycle;
			t_color	**cycle_shadow;
		}	t_spr;
	*/


	free_json_content(lines);
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
