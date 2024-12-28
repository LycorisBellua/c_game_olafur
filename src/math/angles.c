#include "cimmerian.h"

double	atan2(double y, double x);

double	deg2rad(double deg)
{
	return (deg * PI_OVER_180);
}

double	rad2deg(double rad)
{
	return (rad * PI_OVER_180_INVERSE);
}

double	get_angle_from_dir(double dir_x, double dir_y)
{
	if (dir_x != 0 || dir_y != 0)
		return (atan2(dir_y, dir_x));
	return (0);
}
