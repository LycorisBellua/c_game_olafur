#include "cimmerian.h"

void	audio_source_delete(void *data)
{
	t_a_source	*s;

	s = (t_a_source *)data;
	if (!s)
		return ;
	audio_source_unset_track(s);
	if (s->is_loop)
		pthread_mutex_destroy(&s->is_looping_mutex);
	alDeleteSources(1, &s->source);
	free(s);
	return ;
}
