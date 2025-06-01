#include "cimmerian.h"

static int	add_image_slot(t_man *man);
static void	fill_image_slot(t_man *man, int index, char *abs_path, t_png *file);

/*
	If you cannot get the absolute path, it might be because `path` is actually 
	an ID.
*/
t_img	*add_new_image(t_man *man, const char *path)
{
	char	*abs_path;
	t_img	*img;
	t_png	*file;
	int		index;

	abs_path = get_absolute_path(path);
	img = get_image(man, !abs_path ? path : abs_path);
	if (img || !abs_path)
	{
		free(abs_path);
		return (img);
	}
	file = load_png_from_path(abs_path);
	if (!file)
	{
		free(abs_path);
		return (0);
	}
	index = add_image_slot(man);
	if (index >= 0)
		fill_image_slot(man, index, abs_path, file);
	free(abs_path);
	free_png(file);
	return (index < 0 ? 0 : man->images[index]);
}

static int	add_image_slot(t_man *man)
{
	int		count;
	t_img	**new_arr;

	count = count_arr_elems((void **)man->images);
	new_arr = calloc(count + 1 + 1, sizeof(t_img *));
	if (!new_arr)
		return (-1);
	memcpy(new_arr, man->images, count * sizeof(t_img *));
	free(man->images);
	man->images = new_arr;
	return (count);
}

static void	fill_image_slot(t_man *man, int index, char *abs_path, t_png *file)
{
	man->images[index] = create_empty_image(abs_path, file->size);
	if (man->images[index])
	{
		memcpy(man->images[index]->cycle[0], file->buf,
			file->size.x * file->size.y * sizeof(t_color));
		calculate_image_average_color(man->images[index]);
	}
	return ;
}	
