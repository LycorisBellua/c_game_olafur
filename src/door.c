#include "olafur.h"

static void		add_doors_already_opened(t_map *map, t_list **list);
static void		close_doors(t_man *man, t_map *map, t_list **opened_doors,
					int max_dist);
static void		open_new_door(t_man *man, t_list **opened_doors, int max_dist);
static t_door	*get_door(t_map *m, int x, int y);
static int		is_player_next_to_face(t_door *d, t_ivec2 p);
static void		add_door_to_list(t_list **list, t_door *door);
static void		trigger_door(t_door *d, int is_open);

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
		if (d && !d->is_open && is_player_next_to_face(d, p))
		{
			add_door_to_list(opened_doors, d);
			break ;
		}
		d = get_door(m, p.x, p.y + i);
		if (d && !d->is_open && is_player_next_to_face(d, p))
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

static int	is_player_next_to_face(t_door *d, t_ivec2 p)
{
	if (d->cardinal == 'N')
		return (p.y < d->pos.y);
	else if (d->cardinal == 'S')
		return (p.y > d->pos.y);
	else if (d->cardinal == 'W')
		return (p.x < d->pos.x);
	else if (d->cardinal == 'E')
		return (p.x > d->pos.x);
	return (1);
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

static void	trigger_door(t_door *d, int is_open)
{
	t_cell	*c;

	d->is_open = is_open;
	c = &d->m->cells[d->pos.y][d->pos.x];
	c->is_obstacle = !d->is_open;
	c->is_visible = (d->is_open && d->tex_open)
		|| (!d->is_open && d->tex_closed);
	return ;
}
