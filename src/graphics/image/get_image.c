#include "cimmerian.h"

t_img	*get_image(t_man *man, const char *id)
{
	size_t	i;

	i = 0;
	while (man->images[i].id)
	{
		if (!strcmp(man->images[i].id, id))
			return (&man->images[i]);
		++i;
	}
	if (!strcmp(man->map->skybox->id, id))
		return (man->map->skybox);
	return (0);
}
