#include "cimmerian.h"

static void	easter_egg(t_man *man);

void	konami_code(t_man *man, int pressed_key)
{
	static int	sequence[10];
	static int	index;

	if (!sequence[0])
	{
		sequence[0] = KEY_UP;
		sequence[1] = KEY_UP;
		sequence[2] = KEY_DOWN;
		sequence[3] = KEY_DOWN;
		sequence[4] = KEY_LEFT;
		sequence[5] = KEY_RIGHT;
		sequence[6] = KEY_LEFT;
		sequence[7] = KEY_RIGHT;
		sequence[8] = KEY_B;
		sequence[9] = KEY_A;
	}
	if (pressed_key != sequence[index++])
		index = 0;
	if (index == sizeof(sequence) / sizeof(int))
	{
		index = 0;
		easter_egg(man);
	}
	return ;
}

static void	easter_egg(t_man *man)
{
	// TODO: Make proper easter egg
	if (!man->audio.music)
		return ;
	else if (man->audio.music->is_running)
		audio_source_pause(man->audio.music);
	else
		audio_source_play(man->audio.music);
	return ;
}
