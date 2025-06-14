#include "cimmerian.h"

void	audio_source_end_loop_thread(t_a_source *s);

void	audio_source_unset_track(t_a_source *s)
{
	ALint	queued;
	ALuint	buffer;

	if (!s)
		return ;
	if (s->loop_thread)
		audio_source_end_loop_thread(s);
	s->track = 0;
	alSourceStop(s->source);
	queued = 0;
	alGetSourcei(s->source, AL_BUFFERS_QUEUED, &queued);
	while (queued-- > 0)
		alSourceUnqueueBuffers(s->source, 1, &buffer);
	return ;
}
