#include "olafur.h"

void	audio_source_change_track(t_a_source *s, t_a_track *t)
{
	if (!s)
		return ;
	audio_source_unset_track(s);
	if (t)
		alSourceQueueBuffers(s->source, t->nbr_buffers, t->buffers);
	s->track = t;
	return ;
}
