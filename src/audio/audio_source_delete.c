#include "cimmerian.h"

void	audio_source_delete(t_a_source **s)
{
	if (!*s)
		return ;
	audio_source_unset_track(*s);
	if ((*s)->is_loop)
		pthread_mutex_destroy(&(*s)->is_looping_mutex);
	alDeleteSources(1, &(*s)->source);
	free(*s);
	*s = 0;
	return ;
}
