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

/*
	When the user clicks outside of the window (or on the window's header), the 
	app loses focus of the window. While the focus is lost, the window doesn't 
	catch events anymore.

	This is relevant when a key is being pressed, and there's focus loss before 
	the corresponding key release event can happen. The key release is lost, 
	making the player keep moving. It keeps moving while out of focus, and also 
	once it's in focus once more, and there's no way to make the player stop.

	This is where this function comes in: when focus is lost, cancel all input.
*/
int	focus_loss_callback(t_man *man)
{
	int	i;

	i = 0;
	while (man->keys[i].key)
	{
		execute_input_action(man, man->keys[i].key, KEY_STATE_RELEASED);
		++i;
	}
	return (0);
}
