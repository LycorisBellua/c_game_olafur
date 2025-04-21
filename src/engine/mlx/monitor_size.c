#include "cimmerian.h"

t_ivec2	get_monitor_size(t_man *man)
{
	t_ivec2	size;

	mlx_get_screen_size(man->mlx, &size.x, &size.y);
	return (size);
}
