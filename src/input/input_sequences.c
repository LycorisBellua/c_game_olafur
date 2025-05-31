#include "cimmerian.h"

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
		(void)man;
		printf("Konami code!\n");
	}
	return ;
}
