#include "cimmerian.h"

static void	close_last_door(t_map *m, t_list **opened_doors, int max_dist);
static void	open_new_door(t_map *m, t_list **opened_doors, int max_dist);
static int	is_door_and_obstacle(t_map *m, int x, int y);
static void	add_door_to_list(t_map *m, int x, int y, t_list **list);

void	door_routine(t_map *m)
{
	static t_list	*opened_doors;

	close_last_door(m, &opened_doors, 2);
	open_new_door(m, &opened_doors, 2);
	return ;
}

static void	close_last_door(t_map *m, t_list **opened_doors, int max_dist)
{
	t_list	*head;
	t_list	*curr;
	t_ivec2	player;
	t_ivec2	door;

	set_ivec2(&player, (int)g_man.player.pos.x, (int)g_man.player.pos.y);
	curr = *opened_doors;
	head = curr;
	while (curr)
	{
		door = *((t_ivec2 *)curr->data);
		if (!is_within_threshold(player, door, max_dist))
		{
			m->cells[door.y * m->size.x + door.x].is_obstacle = 1;
			if (curr == head)
				head = (curr)->next;
			list_del_one(&curr, free);
		}
		else
			curr = curr->next;
	}
	*opened_doors = head;
	return ;
}

static void	open_new_door(t_map *m, t_list **opened_doors, int max_dist)
{
	int		i;
	t_ivec2	player;

	set_ivec2(&player, (int)g_man.player.pos.x, (int)g_man.player.pos.y);
	i = -max_dist;
	while (i <= max_dist)
	{
		if (is_door_and_obstacle(m, player.x + i, player.y))
		{
			add_door_to_list(m, player.x + i, player.y, opened_doors);
			break ;
		}
		else if (is_door_and_obstacle(m, player.x, player.y + i))
		{
			add_door_to_list(m, player.x, player.y + i, opened_doors);
			break ;
		}
		++i;
	}
	return ;
}

static int	is_door_and_obstacle(t_map *m, int x, int y)
{
	return (m->cells[y * m->size.x + x].is_door
		&& m->cells[y * m->size.x + x].is_obstacle);
}

static void	add_door_to_list(t_map *m, int x, int y, t_list **list)
{
	t_ivec2	*coord;
	t_list	*node;

	coord = calloc(1, sizeof(t_ivec2));
	if (coord)
	{
		set_ivec2(coord, x, y);
		node = list_new(coord);
		if (!node)
			free(coord);
		else
			list_add_back(list, node);
	}
	m->cells[y * m->size.x + x].is_obstacle = 0;
	return ;
}
