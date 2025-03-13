#include "cimmerian.h"

void	decrease_minimap_zoom(t_man *man)
{
	int	zoom;

	zoom = man->map->minimap_zoom;
	if (zoom > 1 && zoom <= 4)
		zoom -= 1;
	else if (zoom == 6)
		zoom -= 2;
	else if (zoom == 9 || zoom == 12)
		zoom -= 3;
	else if (zoom == 18)
		zoom -= 6;
	man->map->minimap_zoom = zoom;
	man->map->minimap_cell_amount = man->map->minimap_radius / zoom * 2;
	return ;
}

void	increase_minimap_zoom(t_man *man)
{
	int	zoom;

	zoom = man->map->minimap_zoom;
	if (zoom > 0 && zoom <= 3)
		zoom += 1;
	else if (zoom == 4)
		zoom += 2;
	else if (zoom == 6 || zoom == 9)
		zoom += 3;
	else if (zoom == 12)
		zoom += 6;
	man->map->minimap_zoom = zoom;
	man->map->minimap_cell_amount = man->map->minimap_radius / zoom * 2;
	return ;
}
