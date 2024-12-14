#include "cimmerian.h"

void	scroll_callback(GLFWwindow *window, double x_offset, double y_offset)
{
	(void)window;
	(void)x_offset;
	update_dof(g_man.map, y_offset);
	set_minimap_display(y_offset);
	return ;
}
