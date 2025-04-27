#include "cimmerian.h"

static t_key		*find_key(t_man *man, int keycode);
static t_key_state	get_new_key_state(t_key *key, t_key_state new_state);
static void			handle_pressed(t_man *man, t_key *key);
static void			handle_held(t_man *man, t_key *key);
static void			handle_released(t_man *man, t_key *key);
static void			handle_none(t_man *man, t_key *key);

void	execute_input_action(t_man *man, int keycode, t_key_state state)
{
	t_key	*key;

	key = find_key(man, keycode);
	if (!key)
		return ;
	state = get_new_key_state(key, state);
	if (state == KEY_STATE_PRESSED)
		handle_pressed(man, key);
	else if (state == KEY_STATE_HELD)
		handle_held(man, key);
	else if (state == KEY_STATE_RELEASED)
		handle_released(man, key);
	else
		handle_none(man, key);
	key->state = state;
	return ;
}

static t_key	*find_key(t_man *man, int keycode)
{
	int	i;

	i = 0;
	while (man->keys[i].keycode && man->keys[i].keycode != keycode)
		++i;
	if (!man->keys[i].keycode)
		return (0);
	return (&man->keys[i]);
}

static t_key_state	get_new_key_state(t_key *key, t_key_state new_state)
{
	if (!key)
		return (KEY_STATE_NONE);
	else if (new_state == KEY_STATE_PRESSED
		&& (key->state == KEY_STATE_PRESSED || key->state == KEY_STATE_HELD))
		new_state = KEY_STATE_HELD;
	return (new_state);
}

static void	handle_pressed(t_man *man, t_key *key)
{
	if (key->actions[man->game_state - 1][KEY_STATE_PRESSED - 1])
		key->actions[man->game_state - 1][KEY_STATE_PRESSED - 1](man, 1);
	return ;
}

static void	handle_held(t_man *man, t_key *key)
{
	if (key->state == KEY_STATE_PRESSED
		&& key->actions[man->game_state - 1][KEY_STATE_PRESSED - 1]
			!= key->actions[man->game_state - 1][KEY_STATE_HELD - 1])
	{
		if (key->actions[man->game_state - 1][KEY_STATE_PRESSED - 1])
			key->actions[man->game_state - 1][KEY_STATE_PRESSED - 1](man, 0);
		if (key->actions[man->game_state - 1][KEY_STATE_HELD - 1])
			key->actions[man->game_state - 1][KEY_STATE_HELD - 1](man, 1);
	}
	return ;
}

static void	handle_released(t_man *man, t_key *key)
{
	if (key->state == KEY_STATE_PRESSED
		&& key->actions[man->game_state - 1][KEY_STATE_PRESSED - 1]
			!= key->actions[man->game_state - 1][KEY_STATE_RELEASED - 1])
	{
		if (key->actions[man->game_state - 1][KEY_STATE_PRESSED - 1])
			key->actions[man->game_state - 1][KEY_STATE_PRESSED - 1](man, 0);
		if (key->actions[man->game_state - 1][KEY_STATE_RELEASED - 1])
			key->actions[man->game_state - 1][KEY_STATE_RELEASED - 1](man, 1);
	}
	else if (key->state == KEY_STATE_HELD
		&& key->actions[man->game_state - 1][KEY_STATE_HELD - 1]
			!= key->actions[man->game_state - 1][KEY_STATE_RELEASED - 1])
	{
		if (key->actions[man->game_state - 1][KEY_STATE_HELD - 1])
			key->actions[man->game_state - 1][KEY_STATE_HELD - 1](man, 0);
		if (key->actions[man->game_state - 1][KEY_STATE_RELEASED - 1])
			key->actions[man->game_state - 1][KEY_STATE_RELEASED - 1](man, 1);
	}
	return ;
}

static void	handle_none(t_man *man, t_key *key)
{
	if (key->state == KEY_STATE_PRESSED)
	{
		if (key->actions[man->game_state - 1][KEY_STATE_PRESSED - 1])
			key->actions[man->game_state - 1][KEY_STATE_PRESSED - 1](man, 0);
	}
	else if (key->state == KEY_STATE_HELD)
	{
		if (key->actions[man->game_state - 1][KEY_STATE_HELD - 1])
			key->actions[man->game_state - 1][KEY_STATE_HELD - 1](man, 0);
	}
	else if (key->state == KEY_STATE_RELEASED)
	{
		if (key->actions[man->game_state - 1][KEY_STATE_RELEASED - 1])
			key->actions[man->game_state - 1][KEY_STATE_RELEASED - 1](man, 0);
	}
	return ;
}
