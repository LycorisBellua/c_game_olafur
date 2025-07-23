#include "olafur.h"

static t_ivec2	get_direction(t_ivec2 v1, t_ivec2 v2);
static t_color	get_lerp_c(t_color c1, t_color c2, int steps, int total_steps);

void	draw_line(t_man *man, t_vert v1, t_vert v2)
{
	int		steps;
	int		total_steps;
	t_ivec2	dir;
	t_vec2	coord;
	t_vec2	increment;

	dir = get_direction(v1.coord, v2.coord);
	steps = imax(abs(dir.x), abs(dir.y));
	coord.x = v1.coord.x;
	coord.y = v1.coord.y;
	increment.x = dir.x / (float)steps;
	increment.y = dir.y / (float)steps;
	++steps;
	total_steps = steps;
	while (steps-- > 0)
	{
		draw_point(man, get_lerp_c(v1.color, v2.color, steps, total_steps),
			coord.x, coord.y);
		coord.x += increment.x;
		coord.y += increment.y;
	}
	return ;
}

static t_ivec2	get_direction(t_ivec2 v1, t_ivec2 v2)
{
	t_ivec2	dir;

	if (v1.x < 0 && v2.x < 0)
		dir.x = abs(v1.x) + v2.x;
	else
		dir.x = v2.x - v1.x;
	if (v1.y < 0 && v2.y < 0)
		dir.y = abs(v1.y) + v2.y;
	else
		dir.y = v2.y - v1.y;
	return (dir);
}

static t_color	get_lerp_c(t_color c1, t_color c2, int steps, int total_steps)
{
	t_color	color;
	float	factor;

	if (total_steps < 1)
		return (c1);
	factor = (float)steps / total_steps;
	color.r = c1.r * (1.0 - factor) + c2.r * factor;
	color.g = c1.g * (1.0 - factor) + c2.g * factor;
	color.b = c1.b * (1.0 - factor) + c2.b * factor;
	color.a = c1.a * (1.0 - factor) + c2.a * factor;
	return (color);
}
