#include "cimmerian.h"

void	audio_track_delete_all(t_audio *a)
{
	int	i;

	if (!a->tracks)
		return ;
	i = 0;
	while (a->tracks[i])
	{
		audio_track_delete(a, &a->tracks[i]);
		++i;
	}
	free(a->tracks);
	a->tracks = 0;
	return ;
}
