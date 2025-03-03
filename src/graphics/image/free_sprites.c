#include "cimmerian.h"

void	free_sprites(void)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < SPRITE_LEN)
	{
		free(g_man.sprites[i].id);
		j = 0;
		while (j < g_man.sprites[i].cycle_len)
		{
			if (g_man.sprites[i].cycle)
				free(g_man.sprites[i].cycle[j]);
			if (g_man.sprites[i].cycle_shadow)
				free(g_man.sprites[i].cycle_shadow[j]);
			++j;
		}
		free(g_man.sprites[i].cycle);
		free(g_man.sprites[i].cycle_shadow);
		free(g_man.sprites[i].average_color);
		free(g_man.sprites[i].is_see_through);
		bzero(g_man.sprites + i, sizeof(t_spr));
		++i;
	}
	return ;
}
