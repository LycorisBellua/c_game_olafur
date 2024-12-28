#include "cimmerian.h"

/* Euclidean distance (all directions) */
double	get_dist_euclidean(double ax, double ay, double bx, double by)
{
	return (f_sqrt(get_squared_dist_euclidean(ax, ay, bx, by)));
}

double	get_squared_dist_euclidean(double ax, double ay, double bx, double by)
{
	return (f_pow(ax - bx, 2) + f_pow(ay - by, 2));
}

/* Manhattan distance (no diagonal) */
double	get_dist_manhattan(double ax, double ay, double bx, double by)
{
	return (f_abs(ax - bx) + f_abs(ay - by));
}
