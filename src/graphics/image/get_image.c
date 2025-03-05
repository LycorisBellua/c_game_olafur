#include "cimmerian.h"

t_img	*get_image(const char *id)
{
	size_t	i;

	i = 0;
	while (g_man.images[i].id)
	{
		if (!strcmp(g_man.images[i].id, id))
			return (&g_man.images[i]);
		++i;
	}
	if (!strcmp(g_man.map->skybox->id, id))
		return (g_man.map->skybox);
	return (0);
}
