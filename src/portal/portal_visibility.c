#include "olafur.h"

int	is_portal_visible(t_portal *portal)
{
	if (!portal || portal->is_corridor)
		return (0);
	if (portal->is_open)
		return (portal->tex_open && portal->tex_open->id);
	return (portal->tex_closed && portal->tex_closed->id);
}
