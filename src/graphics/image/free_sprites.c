#include "cimmerian.h"

void	free_sprites(void)
{
	size_t	i;

	i = 0;
	while (i < SPRITE_LEN)
	{
		free_sprite(g_man.sprites + i);
		++i;
	}
	return ;
}

void	free_sprite(t_spr *s)
{
	if (!s)
		return ;
	free(s->id);
	s->cycle_index = 0;
	while (s->cycle_index < s->cycle_len)
	{
		if (s->cycle)
			free(s->cycle[s->cycle_index]);
		if (s->cycle_shadow)
			free(s->cycle_shadow[s->cycle_index]);
		++s->cycle_index;
	}
	free(s->cycle);
	free(s->cycle_shadow);
	free(s->average_color);
	free(s->is_see_through);
	bzero(s, sizeof(t_spr));
	return ;
}
