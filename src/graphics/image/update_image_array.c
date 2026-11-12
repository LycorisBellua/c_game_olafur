#include "olafur.h"

static int		load_json_file(t_man *man, const char *path);
static int		allocate_array(t_man *man, char **lines, size_t *i_img);
static int		set_pixel_data(t_man *man, t_png *file, size_t *i_img);
static t_color	*get_pixel_data(const char *path, t_ivec2 *size);

/*
	The path is made absolute, so that the same JSON file reached through
	different relative paths is still only loaded once.
*/
int	update_image_array(t_man *man, const char *path)
{
	char	*abs_path;
	int		is_success;

	if (!path)
		return (1);
	abs_path = get_absolute_path(path);
	if (!abs_path)
		return (0);
	is_success = 1;
	if (!register_json_path(man, abs_path))
		is_success = load_json_file(man, abs_path);
	free(abs_path);
	return (is_success);
}

/*
	i[0] is the current line of the JSON file, i[1] the next image slot.
	On failure, only the images added by this call are discarded, since the
	ones loaded before may already be in use by maps.
*/
static int	load_json_file(t_man *man, const char *path)
{
	char	**lines;
	t_png	file;
	size_t	i[2];
	size_t	old_count;
	int		is_success;

	old_count = count_arr_elems((void **)man->images);
	i[0] = 0;
	i[1] = old_count;
	lines = get_json_content(man, path);
	is_success = lines && allocate_array(man, lines, &i[1]);
	memset(&file, 0, sizeof(t_png));
	while (is_success && set_png_file_obj(&file, lines, &i[0]))
	{
		is_success = set_pixel_data(man, &file, &i[1]);
		free_and_reset_png_file_obj(&file);
	}
	free_and_reset_png_file_obj(&file);
	free_json_content(lines);
	if (!is_success)
		discard_new_images(man, old_count, path);
	else
		discard_new_images(man, i[1], 0);
	return (is_success);
}

static int	allocate_array(t_man *man, char **lines, size_t *i_img)
{
	size_t	i;
	size_t	count;
	t_img	**new_arr;

	i = 0;
	count = 0;
	while (lines[i])
		count += is_field(lines[i++], "id");
	if (!count)
		return (put_error(E_IMG_NO_ID, 0, 0));
	new_arr = calloc(*i_img + count + 1, sizeof(t_img *));
	if (!new_arr)
		return (put_error(E_FAIL_MEM, 0, 0));
	if (man->images)
		memcpy(new_arr, man->images, *i_img * sizeof(t_img *));
	free(man->images);
	man->images = new_arr;
	i = 0;
	while (i < count)
	{
		man->images[*i_img + i] = calloc(1, sizeof(t_img));
		if (!man->images[*i_img + i])
			return (put_error(E_FAIL_MEM, 0, 0));
		++i;
	}
	return (1);
}

static int	set_pixel_data(t_man *man, t_png *file, size_t *i_img)
{
	t_ivec2	size_shadow;

	if (!file->path)
		return (put_error(E_IMG_NO_PATH, 0, 0));
	file->buf = get_pixel_data(file->path, &file->size);
	if (!file->buf)
		return (0);
	if (file->path_shadow)
	{
		file->buf_shadow = get_pixel_data(file->path_shadow, &size_shadow);
		if (!file->buf_shadow)
			return (0);
		if (size_shadow.x != file->size.x || size_shadow.y != file->size.y)
			return (put_error(E_IMG_SHADOW_SIZE, file->path_shadow, 0));
	}
	if (!check_png_segments(file))
		return (0);
	return (create_images_from_file(man, file, i_img));
}

/* The pixels are taken over from LodePNG's buffer instead of being copied. */
static t_color	*get_pixel_data(const char *path, t_ivec2 *size)
{
	t_png	*png;
	t_color	*data;

	png = load_png_from_path(path);
	if (!png)
		return (0);
	set_ivec2(size, png->size.x, png->size.y);
	data = png->buf;
	png->buf = 0;
	free_png(png);
	return (data);
}
