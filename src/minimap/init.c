#include "cimmerian.h"

void	init_minimap_values(t_man *man)
{
	man->minimap_radius = man->res.res.y / 10;
	man->minimap_zoom = 9;
	man->minimap_cell_amount = man->minimap_radius / man->minimap_zoom * 2;
	man->minimap_offset.x = man->res.res.x - (man->res.res.x / 640 * 77);
	man->minimap_offset.y = man->res.res.y / 360 * 7;
	man->minimap_center.x = man->minimap_offset.x + man->minimap_radius;
	man->minimap_center.y = man->minimap_offset.y + man->minimap_radius;
	return ;
}
