#include "olafur.h"

void	audio_init(t_audio *a)
{
	a->device = alcOpenDevice(0);
	a->context = alcCreateContext(a->device, 0);
	alcMakeContextCurrent(a->context);
	a->sources[SOUND_MUSIC] = audio_source_create(0, 1);
	a->sources[SOUND_AMBIENCE] = audio_source_create(0, 1);
	a->sources[SOUND_SUCCESS] = audio_source_create(0, 0);
	a->sources[SOUND_FAILURE] = audio_source_create(0, 0);
	return ;
}
