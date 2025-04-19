#include "cimmerian.h"

void	init_minimap_values(t_man *man)
{
	set_ivec2(&man->minimap_offset, 563, 7);
	set_ivec2(&man->minimap_center, 599, 43);
	man->minimap_radius = 36;
	man->minimap_zoom = 9;
	man->minimap_cell_amount = man->minimap_radius / man->minimap_zoom * 2;
}
