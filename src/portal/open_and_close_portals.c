#include "cimmerian.h"

static void		close_portals(t_man *man, t_map *map, t_list **opened_portals,
					int max_dist);
static void		open_new_portal(t_man *man, t_list **opened_portals,
					int max_dist);
static t_portal	*get_portal_ptr(t_map *m, int x, int y);
static void		add_portal_to_list(t_list **list, t_portal *portal);

void	open_and_close_portals(t_man *man)
{
	static t_list	*opened_portals;

	close_portals(man, man->maps[man->curr_map], &opened_portals, 2);
	open_new_portal(man, &opened_portals, 2);
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
			pt->is_open = 0;
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
		pt = get_portal_ptr(m, p.x + i, p.y);
		if (pt && !pt->is_open)
		{
			add_portal_to_list(opened_portals, pt);
			break ;
		}
		pt = get_portal_ptr(m, p.x, p.y + i);
		if (pt && !pt->is_open)
		{
			add_portal_to_list(opened_portals, pt);
			break ;
		}
		++i;
	}
	return ;
}

static t_portal	*get_portal_ptr(t_map *m, int x, int y)
{
	if (x < 0 || x >= m->size.x || y < 0 || y >= m->size.y)
		return (0);
	return (m->cells[y][x].portal);
}

static void	add_portal_to_list(t_list **list, t_portal *portal)
{
	t_list	*node;

	node = list_new(portal);
	if (!node)
		return ;
	list_add_back(list, node);
	portal->is_open = 1;
	return ;
}
