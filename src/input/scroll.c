#include "cimmerian.h"

void	scroll_callback(GLFWwindow *window, double x_offset, double y_offset)
{
	(void)window;
	(void)x_offset;
	//update_dof(g_man.map, y_offset);
	if (y_offset < 0)
		decrease_minimap_zoom();
	else if (y_offset > 0)
		increase_minimap_zoom();
	return ;
}
