#include "cimmerian.h"

double	deg2rad(double deg)
{
	return (deg * PI_OVER_180);
}

double	get_angle_from_dir(t_vec2 dir)
{
	return (atan2(dir.y, dir.x));
}

double	norm_angle(double angle)
{
	while (angle < 0)
		angle += RAD_360;
	while (angle >= RAD_360)
		angle -= RAD_360;
	return (angle);
}

t_vec2	rotate_vec2(t_vec2 v, double angle)
{
	t_vec2	rotated;

	rotated.x = v.x * cos(angle) - v.y * sin(angle);
	rotated.y = v.x * sin(angle) + v.y * cos(angle);
	return (rotated);
}
