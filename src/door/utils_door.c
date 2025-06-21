#include "olafur.h"

t_door	*get_door(t_map *m, int x, int y)
{
	if (x < 0 || x >= m->size.x || y < 0 || y >= m->size.y)
		return (0);
	return (m->cells[y][x].door);
}

int	is_player_next_to_door_face(t_door *d, t_ivec2 p)
{
	if (d->opening_cardinal == 'N')
		return (p.y < d->pos.y);
	else if (d->opening_cardinal == 'S')
		return (p.y > d->pos.y);
	else if (d->opening_cardinal == 'W')
		return (p.x < d->pos.x);
	else if (d->opening_cardinal == 'E')
		return (p.x > d->pos.x);
	return (1);
}

void	trigger_door(t_door *d, int is_open)
{
	t_cell	*c;

	d->is_open = is_open;
	c = &d->m->cells[d->pos.y][d->pos.x];
	c->is_obstacle = !d->is_open;
	c->is_visible = (d->is_open && d->tex_open)
		|| (!d->is_open && d->tex_closed);
	return ;
}
