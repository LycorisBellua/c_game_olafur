#include "cimmerian.h"

static void	advance_sprite_cycle(t_spr *sprite, long dt_ms);

void	advance_all_sprite_cycles(long dt_ms)
{
	size_t	i;

	i = 0;
	while (g_man.sprites[i].id)
	{
		advance_sprite_cycle(g_man.sprites + i, dt_ms);
		++i;
	}
	return ;
}

static void	advance_sprite_cycle(t_spr *sprite, long dt_ms)
{
	if (!sprite || !dt_ms || !sprite->cycle_time_in_ms)
		return ;
	sprite->elapsed_time_in_ms += dt_ms;
	while (sprite->elapsed_time_in_ms >= sprite->cycle_time_in_ms)
	{
		sprite->cycle_index = (sprite->cycle_index + 1) % sprite->cycle_len;
		sprite->elapsed_time_in_ms -= sprite->cycle_time_in_ms;
	}
	return ;
}
