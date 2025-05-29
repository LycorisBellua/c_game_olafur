#include "cimmerian.h"

static void		close_doors(t_man *man, t_map *map, t_list **opened_doors,
					int max_dist);
static void		open_new_door(t_man *man, t_list **opened_doors, int max_dist);
static t_door	*get_door(t_map *m, int x, int y);
static void		add_door_to_list(t_list **list, t_door *door);

void	door_routine(t_man *man)
{
	static t_list	*opened_doors;

	close_doors(man, man->maps[man->curr_map], &opened_doors, 2);
	open_new_door(man, &opened_doors, 2);
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
			d->is_open = 0;
			*d->is_obstacle = 1;
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
		if (d && !d->is_open)
		{
			add_door_to_list(opened_doors, d);
			break ;
		}
		d = get_door(m, p.x, p.y + i);
		if (d && !d->is_open)
		{
			add_door_to_list(opened_doors, d);
			break ;
		}
		++i;
	}
	return ;
}

static t_door	*get_door(t_map *m, int x, int y)
{
	if (x < 0 || x >= m->size.x || y < 0 || y >= m->size.y)
		return (0);
	return (m->cells[y][x].door);
}

static void	add_door_to_list(t_list **list, t_door *door)
{
	t_list	*node;

	node = list_new(door);
	if (!node)
		return ;
	list_add_back(list, node);
	door->is_open = 1;
	*door->is_obstacle = 0;
	return ;
}
