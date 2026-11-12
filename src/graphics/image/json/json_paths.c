#include "olafur.h"

/*
	Returns 1 if the JSON file at `path` was already loaded. Otherwise, the
	path is registered and 0 is returned.
*/
int	register_json_path(t_man *man, const char *path)
{
	size_t	i;
	char	**new_arr;

	i = 0;
	while (man->json_paths && man->json_paths[i])
	{
		if (!strcmp(man->json_paths[i], path))
			return (1);
		++i;
	}
	new_arr = calloc(i + 1 + 1, sizeof(char *));
	if (!new_arr)
		return (0);
	if (man->json_paths)
		memcpy(new_arr, man->json_paths, i * sizeof(char *));
	free(man->json_paths);
	man->json_paths = new_arr;
	man->json_paths[i] = strdup(path);
	return (0);
}

/*
	Frees the images from index `start` onward. If `path` is given, loading its
	JSON file failed, so it's unregistered to allow a later attempt.
*/
void	discard_new_images(t_man *man, size_t start, const char *path)
{
	size_t	i;

	i = start;
	while (man->images && man->images[i])
	{
		free_image(man->images[i], free);
		man->images[i] = 0;
		++i;
	}
	i = count_arr_elems((void **)man->json_paths);
	if (path && i && !strcmp(man->json_paths[i - 1], path))
	{
		free(man->json_paths[i - 1]);
		man->json_paths[i - 1] = 0;
	}
	return ;
}
