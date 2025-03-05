#include "cimmerian.h"

double	sqrt_f(double n)
{
	double	x;
	double	xhalf;
	int64_t	i;

	x = n;
	xhalf = 0.5 * x;
	i = *(int64_t *)&x;
	i = 0x5fe6ec85e7de30da - (i >> 1);
	x = *(double *)&i;
	x = x * (1.5 - xhalf * x * x);
	x = x * (1.5 - xhalf * x * x);
	x = x * (1.5 - xhalf * x * x);
	return (x * n);
}

float	sqrt_f32(float n)
{
	float	x;
	float	xhalf;
	int32_t	i;

	x = n;
	xhalf = 0.5f * x;
	i = *(int32_t *)&x;
	i = 0x5f375a86 - (i >> 1);
	x = *(float *)&i;
	x = x * (1.5f - xhalf * x * x);
	x = x * (1.5f - xhalf * x * x);
	x = x * (1.5f - xhalf * x * x);
	return (x * n);
}
