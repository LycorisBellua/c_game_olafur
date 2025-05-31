#include "cimmerian.h"

static void		add_portals_already_opened(t_map *map, t_list **list);
static void		close_portals(t_man *man, t_map *map, t_list **opened_portals,
					int max_dist);
static void		open_new_portal(t_man *man, t_list **opened_portals,
					int max_dist);
static t_portal	*get_portal_ptr(t_map *m, int x, int y);
static int		is_player_next_to_face(t_portal *pt, t_ivec2 p);
static void		add_portal_to_list(t_list **list, t_portal *portal);
static void		trigger_portal(t_portal *p, int is_open);

void	open_and_close_portals(t_man *man)
{
	static t_list	*opened_portals;
	static int		curr_map = -1;
	t_map			*map;

	map = man->maps[man->curr_map];
	if (curr_map != man->curr_map)
	{
		add_portals_already_opened(map, &opened_portals);
		curr_map = man->curr_map;
	}
	close_portals(man, map, &opened_portals, 2);
	open_new_portal(man, &opened_portals, 2);
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
		pt = get_portal_ptr(m, p.x + i, p.y);
		if (pt && !pt->is_open && is_player_next_to_face(pt, p))
		{
			add_portal_to_list(opened_portals, pt);
			break ;
		}
		pt = get_portal_ptr(m, p.x, p.y + i);
		if (pt && !pt->is_open && is_player_next_to_face(pt, p))
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

static int	is_player_next_to_face(t_portal *pt, t_ivec2 p)
{
	if (pt->src_cardinal == 'N')
		return (p.y < pt->src_pos.y);
	else if (pt->src_cardinal == 'S')
		return (p.y > pt->src_pos.y);
	else if (pt->src_cardinal == 'W')
		return (p.x < pt->src_pos.x);
	else if (pt->src_cardinal == 'E')
		return (p.x > pt->src_pos.x);
	return (1);
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

static void	trigger_portal(t_portal *p, int is_open)
{
	t_cell	*c;
	int		is_portal_visible;

	p->is_open = is_open;
	c = &p->src_map->cells[p->src_pos.y][p->src_pos.x];
	is_portal_visible = (p->is_open && p->tex_open)
		|| (!p->is_open && p->tex_closed);
	c->is_visible = is_portal_visible || c->tex_north || c->tex_south
		|| c->tex_west || c->tex_east;
	return ;
}
