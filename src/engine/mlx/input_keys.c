#include "cimmerian.h"

void	key_press_callback(int key, t_man *man)
{
	execute_input_action(man, key, KEY_STATE_PRESSED);
	return ;
}

void	key_release_callback(int key, t_man *man)
{
	execute_input_action(man, key, KEY_STATE_RELEASED);
	return ;
}
