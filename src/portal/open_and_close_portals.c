#include "olafur.h"

static void		add_portals_already_opened(t_map *map, t_list **list);
static void		close_portals(t_man *man, t_map *map, t_list **opened_portals,
					int max_dist);
static void		open_new_portal(t_man *man, t_list **opened_portals,
					int max_dist);
static void		add_portal_to_list(t_list **list, t_portal *portal);

void	open_and_close_portals(t_man *man)
{
	static int	curr_map = -1;
	t_map		*map;

	map = man->maps[man->curr_map];
	if (curr_map != man->curr_map)
	{
		add_portals_already_opened(map, &man->opened_portals);
		curr_map = man->curr_map;
	}
	close_portals(man, map, &man->opened_portals, 1);
	open_new_portal(man, &man->opened_portals, 1);
	return ;
}

static void	add_portals_already_opened(t_map *map, t_list **list)
{
	t_ivec2		i;
	t_portal	*p;

	i.y = 0;
	while (i.y < map->size.y)
	{
		i.x = 0;
		while (i.x < map->size.x)
		{
			p = map->cells[i.y][i.x].portal;
			if (p && p->is_open)
				add_portal_to_list(list, p);
			++i.x;
		}
		++i.y;
	}
	return ;
}

static void	close_portals(t_man *man, t_map *map, t_list **opened_portals,
	int max_dist)
{
	t_list		*head;
	t_list		*curr;
	t_ivec2		p;
	t_portal	*pt;

	set_ivec2(&p, (int)man->player.pos.x, (int)man->player.pos.y);
	curr = *opened_portals;
	head = curr;
	while (curr)
	{
		pt = (t_portal *)curr->data;
		if (pt->src_map != map
			|| !is_dist_grid_aligned_and_within_threshold(p, pt->src_pos,
				max_dist))
		{
			trigger_portal(pt, 0);
			if (curr == head)
				head = (curr)->next;
			list_del_one(&curr, 0);
		}
		else
			curr = curr->next;
	}
	*opened_portals = head;
	return ;
}

static void	open_new_portal(t_man *man, t_list **opened_portals, int max_dist)
{
	int			i;
	t_map		*m;
	t_ivec2		p;
	t_portal	*pt;

	m = man->maps[man->curr_map];
	set_ivec2(&p, (int)man->player.pos.x, (int)man->player.pos.y);
	i = -max_dist;
	while (i <= max_dist)
	{
		pt = get_portal_at(m, p.x + i, p.y);
		if (pt && !pt->is_open && is_player_next_to_portal_face(pt, p))
		{
			add_portal_to_list(opened_portals, pt);
			break ;
		}
		pt = get_portal_at(m, p.x, p.y + i);
		if (pt && !pt->is_open && is_player_next_to_portal_face(pt, p))
		{
			add_portal_to_list(opened_portals, pt);
			break ;
		}
		++i;
	}
	return ;
}

static void	add_portal_to_list(t_list **list, t_portal *portal)
{
	t_list	*node;

	node = list_new(portal);
	if (!node)
		return ;
	list_add_back(list, node);
	trigger_portal(portal, 1);
	return ;
}
