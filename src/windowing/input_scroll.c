#include "olafur.h"

/*
	Trackpads scroll by fractions of a step, so they're added up until they 
	make whole steps.
*/
void	scroll_callback(GLFWwindow *window, double x_offset, double y_offset)
{
	static float	scrolled;
	int				steps;

	(void)window;
	(void)x_offset;
	scrolled += (float)y_offset;
	steps = (int)scrolled;
	scrolled -= steps;
	while (steps > 0)
	{
		increase_minimap_zoom(&g_man);
		--steps;
	}
	while (steps < 0)
	{
		decrease_minimap_zoom(&g_man);
		++steps;
	}
	return ;
}
