#include "olafur.h"

static t_key		*find_key(t_man *man, int keycode);
static t_key_state	get_new_key_state(t_key *key, t_key_state new_state);
static void			switch_action(t_man *man, t_key *key, t_key_state from,
						t_key_state to);

void	execute_input_action(t_man *man, int keycode, t_key_state state)
{
	t_key	*key;

	key = find_key(man, keycode);
	if (!key)
		return ;
	state = get_new_key_state(key, state);
	if (state == KEY_STATE_PRESSED)
		switch_action(man, key, KEY_STATE_NONE, KEY_STATE_PRESSED);
	else if (state == KEY_STATE_HELD && key->state == KEY_STATE_PRESSED)
		switch_action(man, key, KEY_STATE_PRESSED, KEY_STATE_HELD);
	else if (state == KEY_STATE_RELEASED && (key->state == KEY_STATE_PRESSED
			|| key->state == KEY_STATE_HELD))
		switch_action(man, key, key->state, KEY_STATE_RELEASED);
	else if (state == KEY_STATE_NONE)
		switch_action(man, key, key->state, KEY_STATE_NONE);
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

static void	switch_action(t_man *man, t_key *key, t_key_state from,
	t_key_state to)
{
	void	(**actions)(t_man *, int);
	void	(*from_action)(t_man *, int);
	void	(*to_action)(t_man *, int);

	actions = key->actions[man->game_state - 1];
	from_action = 0;
	to_action = 0;
	if (from != KEY_STATE_NONE)
		from_action = actions[from - 1];
	if (to != KEY_STATE_NONE)
		to_action = actions[to - 1];
	if (from != KEY_STATE_NONE && from_action == to_action)
		return ;
	if (from_action)
		from_action(man, 0);
	if (to_action)
		to_action(man, 1);
	return ;
}
