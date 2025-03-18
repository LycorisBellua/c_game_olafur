#include "cimmerian.h"

/* Euclidean distance (all directions) */
double	get_dist(double ax, double ay, double bx, double by)
{
	t_vec2	delta;

	delta.x = ax - bx;
	delta.y = ay - by;
	return (sqrt_f(delta.x * delta.x + delta.y * delta.y));
}

/* Manhattan distance (no diagonal) */
double	get_dist_manhattan(double ax, double ay, double bx, double by)
{
	return (abs_f(ax - bx) + abs_f(ay - by));
}

/*
	Grid-based like Manhattan, and return false if A and B aren't aligned 
	vertically and/or horizontally
*/
int	is_within_threshold(t_ivec2 a, t_ivec2 b, int threshold)
{
	t_ivec2	delta;

	set_ivec2(&delta, abs(a.x - b.x), abs(a.y - b.y));
	if (delta.x > 0 && delta.y > 0)
		return (0);
	return (max(delta.x, delta.y) <= threshold);
}
