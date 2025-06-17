#include "olafur.h"

/* Spatialization only works if the track is mono */
void	audio_source_spatialization(t_a_source *s, int enable)
{
	if (enable)
	{
		alSourcei(s->source, AL_SOURCE_RELATIVE, AL_FALSE);
		alSourcef(s->source, AL_ROLLOFF_FACTOR, 1.0f);
		alSourcef(s->source, AL_REFERENCE_DISTANCE, 1.0f);
		alSourcef(s->source, AL_MAX_DISTANCE, 100.0f);
	}
	else
	{
		alSource3f(s->source, AL_POSITION, 0.0f, 0.0f, 0.0f);
		alSourcei(s->source, AL_SOURCE_RELATIVE, AL_TRUE);
		alSourcef(s->source, AL_ROLLOFF_FACTOR, 0.0f);
		alSourcef(s->source, AL_REFERENCE_DISTANCE, 1.0f);
		alSourcef(s->source, AL_MAX_DISTANCE, FLT_MAX);
	}
	return ;
}
