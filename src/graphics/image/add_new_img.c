#include "cimmerian.h"

t_img	*add_new_image(t_man *man, const char *path)
{
	t_png	*file;
	t_img	*img;
	t_img	**new_arr;
	size_t	count;

	img = get_image(man, path);
	if (img)
		return (img);
	count = count_arr_elems((void **)man->images);
	new_arr = calloc(count + 1 + 1, sizeof(t_img *));
	if (!new_arr)
		return (0);
	memcpy(new_arr, man->images, count * sizeof(t_img *));
	free(man->images);
	man->images = new_arr;
	file = load_png_from_path(path);
	if (file)
		man->images[count] = create_empty_image(path, file->size);
	if (man->images[count])
		memcpy(man->images[count]->cycle[0], file->buf,
			file->size.x * file->size.y * sizeof(t_color));
	free_png(file);
	calculate_image_average_color(man->images[count]);
	return (man->images[count]);
}
