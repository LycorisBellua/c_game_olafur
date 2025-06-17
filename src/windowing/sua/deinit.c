#include "olafur.h"

void	deinit(t_man *man)
{
	if (!man)
		return ;
	if (man->xvar)
		sua_window_destroy(man->xvar);
	free_frame(man);
	sua_display_destroy(man->xvar);
	free(man->xvar);
	man->xvar = 0;
	free_images(man);
	free_maps(man);
	audio_deinit(&man->audio);
	return ;
}
