#include "cimmerian.h"

t_a_source	*audio_source_create(t_a_track *t, int is_loop)
{
	t_a_source	*s;

	s = calloc(1, sizeof(t_a_source));
	if (!s)
		return (0);
	alGenSources(1, &s->source);
	s->track = t;
	if (t)
		alSourceQueueBuffers(s->source, t->nbr_buffers, t->buffers);
	s->is_loop = is_loop;
	if (s->is_loop)
		pthread_mutex_init(&s->is_looping_mutex, 0);
	return (s);
}
