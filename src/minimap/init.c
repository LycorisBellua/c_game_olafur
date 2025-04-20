#include "cimmerian.h"

void	init_minimap_values(t_man *man)
{
	t_ivec2	res;

	res = man->res.window_size_default;
	man->minimap_radius = res.y / 10;
	man->minimap_zoom = 9;
	man->minimap_cell_amount = man->minimap_radius / man->minimap_zoom * 2;
	man->minimap_offset.x = res.x - (res.x / 640 * 77);
	man->minimap_offset.y = res.y / 360 * 7;
	man->minimap_center.x = man->minimap_offset.x + man->minimap_radius;
	man->minimap_center.y = man->minimap_offset.y + man->minimap_radius;
	return ;
}
