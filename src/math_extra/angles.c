#include "olafur.h"

float	deg2rad(float deg)
{
	return (deg * PI_OVER_180);
}

float	get_angle_from_dir(t_vec2 dir)
{
	return (atan2f(dir.y, dir.x));
}

float	norm_angle(float angle)
{
	while (angle < 0)
		angle += RAD_360;
	while (angle >= RAD_360)
		angle -= RAD_360;
	return (angle);
}

float	dot(t_vec2 a, t_vec2 b)
{
	return (a.x * b.x + a.y * b.y);
}

t_vec2	rotate_vec2(t_vec2 v, float angle)
{
	t_vec2	rotated;

	rotated.x = v.x * cosf(angle) - v.y * sinf(angle);
	rotated.y = v.x * sinf(angle) + v.y * cosf(angle);
	return (rotated);
}
