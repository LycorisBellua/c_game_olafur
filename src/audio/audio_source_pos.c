#include "cimmerian.h"

void	audio_source_pos(t_a_source *s, t_vec2 pos)
{
	ALfloat	velocity[3];
	ALfloat	position[3];

	velocity[0] = 0;
	velocity[1] = 0;
	velocity[2] = 0;
	position[0] = (ALfloat)pos.x;
	position[1] = 0;
	position[2] = (ALfloat)pos.y;
	alSourcefv(s->source, AL_VELOCITY, velocity);
	alSourcefv(s->source, AL_POSITION, position);
	return ;
}
