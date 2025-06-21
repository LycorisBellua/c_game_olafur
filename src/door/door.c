#include "olafur.h"

static void		add_doors_already_opened(t_map *map, t_list **list);
static void		close_doors(t_man *man, t_map *map, t_list **opened_doors,
					int max_dist);
static void		open_new_door(t_man *man, t_list **opened_doors, int max_dist);
static void		add_door_to_list(t_list **list, t_door *door);

void	door_routine(t_man *man)
{
	static t_list	*opened_doors;
	static int		curr_map = -1;
	t_map			*map;

	map = man->maps[man->curr_map];
	if (curr_map != man->curr_map)
	{
		add_doors_already_opened(map, &opened_doors);
		curr_map = man->curr_map;
	}
	close_doors(man, map, &opened_doors, 1);
	open_new_door(man, &opened_doors, 1);
	return ;
}

static void	add_doors_already_opened(t_map *map, t_list **list)
{
	t_ivec2	i;
	t_door	*d;

	i.y = 0;
	while (i.y < map->size.y)
	{
		i.x = 0;
		while (i.x < map->size.x)
		{
			d = map->cells[i.y][i.x].door;
			if (d && d->is_open)
				add_door_to_list(list, d);
			++i.x;
		}
		++i.y;
	}
	return ;
}

static void	close_doors(t_man *man, t_map *map, t_list **opened_doors,
	int max_dist)
{
	t_list	*head;
	t_list	*curr;
	t_ivec2	p;
	t_door	*d;

	set_ivec2(&p, (int)man->player.pos.x, (int)man->player.pos.y);
	curr = *opened_doors;
	head = curr;
	while (curr)
	{
		d = (t_door *)curr->data;
		if (d->m != map
			|| !is_dist_grid_aligned_and_within_threshold(p, d->pos, max_dist))
		{
			trigger_door(d, 0);
			if (curr == head)
				head = (curr)->next;
			list_del_one(&curr, 0);
		}
		else
			curr = curr->next;
	}
	*opened_doors = head;
	return ;
}

static void	open_new_door(t_man *man, t_list **opened_doors, int max_dist)
{
	int		i;
	t_map	*m;
	t_ivec2	p;
	t_door	*d;

	m = man->maps[man->curr_map];
	set_ivec2(&p, (int)man->player.pos.x, (int)man->player.pos.y);
	i = -max_dist;
	while (i <= max_dist)
	{
		d = get_door(m, p.x + i, p.y);
		if (d && !d->is_open && is_player_next_to_door_face(d, p))
		{
			add_door_to_list(opened_doors, d);
			break ;
		}
		d = get_door(m, p.x, p.y + i);
		if (d && !d->is_open && is_player_next_to_door_face(d, p))
		{
			add_door_to_list(opened_doors, d);
			break ;
		}
		++i;
	}
	return ;
}

static void	add_door_to_list(t_list **list, t_door *door)
{
	t_list	*node;

	node = list_new(door);
	if (!node)
		return ;
	list_add_back(list, node);
	trigger_door(door, 1);
	return ;
}
