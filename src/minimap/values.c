#include "cimmerian.h"

void	init_minimap_values(t_man *man)
{
	man->minimap_radius = man->res.res.y / 10;
	man->minimap_zoom = 9;
	man->minimap_cell_amount = man->minimap_radius / man->minimap_zoom * 2;
	man->minimap_offset.x = man->res.res.x - 77;
	man->minimap_offset.y = 7;
	man->minimap_center.x = man->minimap_offset.x + man->minimap_radius;
	man->minimap_center.y = man->minimap_offset.y + man->minimap_radius;
	man->minimap_half_90_deg_cos = cos(deg2rad(90) / 2.0);
	return ;
}

void	decrease_minimap_zoom(t_man *man)
{
	int	zoom;

	zoom = man->minimap_zoom;
	if (zoom > 1 && zoom <= 4)
		zoom -= 1;
	else if (zoom == 6)
		zoom -= 2;
	else if (zoom == 9 || zoom == 12)
		zoom -= 3;
	else if (zoom == 18)
		zoom -= 6;
	man->minimap_zoom = zoom;
	man->minimap_cell_amount = man->minimap_radius / zoom * 2;
	return ;
}

void	increase_minimap_zoom(t_man *man)
{
	int	zoom;

	zoom = man->minimap_zoom;
	if (zoom > 0 && zoom <= 3)
		zoom += 1;
	else if (zoom == 4)
		zoom += 2;
	else if (zoom == 6 || zoom == 9)
		zoom += 3;
	else if (zoom == 12)
		zoom += 6;
	man->minimap_zoom = zoom;
	man->minimap_cell_amount = man->minimap_radius / zoom * 2;
	return ;
}
