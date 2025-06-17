#include "olafur.h"

void	audio_deinit(t_audio *a)
{
	audio_track_delete_all(a);
	audio_source_delete(a->sources[SOUND_MUSIC]);
	audio_source_delete(a->sources[SOUND_AMBIENCE]);
	audio_source_delete(a->sources[SOUND_SUCCESS]);
	audio_source_delete(a->sources[SOUND_FAILURE]);
	a->sources[SOUND_MUSIC] = 0;
	a->sources[SOUND_AMBIENCE] = 0;
	a->sources[SOUND_SUCCESS] = 0;
	a->sources[SOUND_FAILURE] = 0;
	alcMakeContextCurrent(0);
	alcDestroyContext(a->context);
	alcCloseDevice(a->device);
	return ;
}
