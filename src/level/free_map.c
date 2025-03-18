#include "cimmerian.h"

void	free_map(t_man *man)
{
	if (!man->map)
		return ;
	free(man->map->cells);
	free_image(man->map->skybox, free);
	free_png(man->map->background);
	free_sprite_array(man);
	free(man->map);
	man->map = 0;
	return ;
}
