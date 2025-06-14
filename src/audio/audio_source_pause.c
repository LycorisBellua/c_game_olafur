#include "cimmerian.h"

void	audio_source_end_loop_thread(t_a_source *s);

void	audio_source_pause(t_a_source *s)
{
	if (!s)
		return ;
	if (s->loop_thread)
		audio_source_end_loop_thread(s);
	alSourcePause(s->source);
	return ;
}
