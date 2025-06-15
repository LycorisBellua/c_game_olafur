#include "cimmerian.h"

void	audio_deinit(t_audio *a)
{
	audio_track_delete_all(a);
	audio_source_delete(&a->sources[SOUND_MUSIC]);
	audio_source_delete(&a->sources[SOUND_AMBIENCE]);
	audio_source_delete(&a->sources[SOUND_SUCCESS]);
	audio_source_delete(&a->sources[SOUND_FAILURE]);
	alcMakeContextCurrent(0);
	alcDestroyContext(a->context);
	alcCloseDevice(a->device);
	return ;
}
