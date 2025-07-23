#include "olafur.h"

float	dist(t_vec2 a, t_vec2 b)
{
	t_vec2	delta;

	set_vec2(&delta, a.x - b.x, a.y - b.y);
	return (sqrt_approx(delta.x * delta.x + delta.y * delta.y));
}

/*
	Grid-based. Returns false if A and B aren't aligned vertically and/or 
	horizontally, or if the distance is not within a certain threshold.
*/
int	is_dist_grid_aligned_and_within_threshold(t_ivec2 a, t_ivec2 b,
	int threshold)
{
	t_ivec2	delta;

	if (threshold < 0)
		return (0);
	set_ivec2(&delta, abs(a.x - b.x), abs(a.y - b.y));
	if (delta.x > 0 && delta.y > 0)
		return (0);
	return (imax(delta.x, delta.y) <= threshold);
}
