#include "olafur.h"

t_img	*get_image(t_man *man, const char *id)
{
	size_t	i;

	if (man->images)
	{
		i = 0;
		while (man->images[i])
		{
			if (!strcmp(man->images[i]->id, id))
				return (man->images[i]);
			++i;
		}
	}
	if (man->maps)
	{
		i = 0;
		while (man->maps[i])
		{
			if (man->maps[i]->skybox
				&& !strcmp(man->maps[i]->skybox->id, id))
				return (man->maps[i]->skybox);
			++i;
		}
	}
	return (0);
}
