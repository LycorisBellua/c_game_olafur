#include "cimmerian.h"

void	audio_init(t_audio *a)
{
	a->device = alcOpenDevice(NULL);
	a->context = alcCreateContext(a->device, NULL);
	alcMakeContextCurrent(a->context);
	return ;
}

void	audio_deinit(t_audio *a)
{
	audio_track_delete_all(a);
	audio_source_delete(&a->music);
	alcDestroyContext(a->context);
	alcCloseDevice(a->device);
	return ;
}

void	audio_switch_music(t_man *man)
{
	t_a_track	*prev_track;
	t_a_track	*new_track;

	if (!man->audio.music)
		return ;
	prev_track = man->audio.music->track;
	new_track = man->maps[man->curr_map]->music_track;
	if (man->audio.music->is_running && prev_track == new_track)
		return ;
	audio_source_change_track(man->audio.music, new_track);
	audio_source_play(man->audio.music);
	return ;
}
