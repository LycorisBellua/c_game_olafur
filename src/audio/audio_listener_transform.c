#include "olafur.h"

void	audio_listener_transform(t_player *p)
{
	ALfloat	velocity[3];
	ALfloat	position[3];
	ALfloat	orientation[6];

	velocity[0] = 0;
	velocity[1] = 0;
	velocity[2] = 0;
	position[0] = (ALfloat)p->pos.x;
	position[1] = 0;
	position[2] = (ALfloat)p->pos.y;
	orientation[0] = (ALfloat)p->dir.x;
	orientation[1] = 0;
	orientation[2] = (ALfloat)p->dir.y;
	orientation[3] = 0;
	orientation[4] = 1.0f;
	orientation[5] = 0;
	alListenerfv(AL_VELOCITY, velocity);
	alListenerfv(AL_POSITION, position);
	alListenerfv(AL_ORIENTATION, orientation);
	return ;
}
