#include "cimmerian.h"

t_png	*load_png_from_path(const char *path)
{
	t_png			*png;
	t_uivec2		size;
	unsigned int	err;

	png = malloc(sizeof(t_png));
	if (!png)
		return (0);
	err = lodepng_decode32_file((t_ubyte **)&png->buf, &size.x, &size.y, path);
	if (err)
	{
		dprintf(2, "Error: Lodepng error n°%u for \"%s\": %s\n",
			err, path, lodepng_error_text(err));
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
