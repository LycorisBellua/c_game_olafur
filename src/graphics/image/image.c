#include "olafur.h"

t_png	*load_png_from_path(const char *path)
{
	t_png			*png;
	t_uivec2		size;
	unsigned int	err;
	char			msg[PATH_MSG_SIZE];

	png = malloc(sizeof(t_png));
	if (!png)
		return (0);
	err = lodepng_decode32_file((t_ubyte **)&png->buf, &size.x, &size.y, path);
	if (err)
	{
		snprintf(msg, PATH_MSG_SIZE, "Lodepng error n°%u for \"%s\"", err,
			path);
		put_error(msg, lodepng_error_text(err), 0);
		free_png(png);
		return (0);
	}
	png->size.x = size.x;
	png->size.y = size.y;
	return (png);
}

void	free_png(t_png *png)
{
	if (png)
		free(png->buf);
	free(png);
	return ;
}
