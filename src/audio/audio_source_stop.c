#include "cimmerian.h"

void	audio_source_end_loop_thread(t_a_source *s);

void	audio_source_stop(t_a_source *s)
{
	if (!s)
		return ;
	if (s->loop_thread)
		audio_source_end_loop_thread(s);
	alSourceStop(s->source);
	return ;
}
