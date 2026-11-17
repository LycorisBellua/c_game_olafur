#include "olafur.h"

t_portal	*get_portal_at(t_map *m, int x, int y)
{
	if (x < 0 || x >= m->size.x || y < 0 || y >= m->size.y)
		return (0);
	return (m->cells[y][x].portal);
}

int	is_player_next_to_portal_face(t_portal *pt, t_ivec2 p)
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

void	trigger_portal(t_portal *p, int is_open)
{
	t_cell	*c;

	p->is_open = is_open;
	c = &p->src_map->cells[p->src_pos.y][p->src_pos.x];
	c->is_visible = is_portal_visible(p) || c->tex_north || c->tex_south
		|| c->tex_west || c->tex_east;
	return ;
}
