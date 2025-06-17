#include "olafur.h"

/*
	Comb through all sources to find which ones hold a reference to this track, 
	and unset their track before deleting this track.
*/
void	audio_track_delete(t_audio *a, t_a_track **track)
{
	int	i;

	i = 0;
	while (++i < 6)
	{
		if (a->sources[i] && a->sources[i]->track == *track)
			audio_source_unset_track(a->sources[i]);
	}
	if ((*track)->buffers)
		alDeleteBuffers((*track)->nbr_buffers, (*track)->buffers);
	free((*track)->buffers);
	free((*track)->name);
	free(*track);
	*track = 0;
	return ;
}
