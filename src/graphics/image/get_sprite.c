#include "cimmerian.h"

t_spr	*get_sprite(char *id)
{
	size_t	i;

	i = 0;
	while (g_man.sprites[i].id)
	{
		if (!strcmp(g_man.sprites[i].id, id))
			return (&g_man.sprites[i]);
		++i;
	}
	return (0);
}
