#include "cimmerian.h"

void	audio_switch_map_sounds(t_man *man)
{
	t_sound_type	type;
	t_a_track		*prev_track;
	t_a_track		*new_track;

	type = 0;
	while (++type < 6)
	{
		if (!man->audio.sources[type])
			continue ;
		prev_track = man->audio.sources[type]->track;
		new_track = man->maps[man->curr_map]->tracks[type];
		if (prev_track == new_track)
			continue ;
		audio_source_change_track(man->audio.sources[type], new_track);
		if (man->audio.sources[type]->is_loop)
			audio_source_play(man->audio.sources[type]);
	}
	return ;
}
