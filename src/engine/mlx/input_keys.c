#include "cimmerian.h"

int	key_press_callback(int key, t_man *man)
{
	execute_input_action(man, key, KEY_STATE_PRESSED);
	return (0);
}

int	key_release_callback(int key, t_man *man)
{
	execute_input_action(man, key, KEY_STATE_RELEASED);
	return (0);
}
