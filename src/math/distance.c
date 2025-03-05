#include "cimmerian.h"

/* Euclidean distance (all directions) */
double	get_dist_euclidean(double ax, double ay, double bx, double by)
{
	return (sqrt_f(get_squared_dist_euclidean(ax, ay, bx, by)));
}

double	get_squared_dist_euclidean(double ax, double ay, double bx, double by)
{
	return (pow_f(ax - bx, 2) + pow_f(ay - by, 2));
}

/* Manhattan distance (no diagonal) */
double	get_dist_manhattan(double ax, double ay, double bx, double by)
{
	return (abs_f(ax - bx) + abs_f(ay - by));
}
