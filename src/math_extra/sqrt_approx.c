#include "cimmerian.h"

double	sqrt_approx(double n)
{
	double	x;
	double	xhalf;
	int64_t	i;

	x = n;
	xhalf = 0.5 * x;
	memcpy(&i, &x, sizeof(int64_t));
	i = 0x5fe6ec85e7de30da - (i >> 1);
	memcpy(&x, &i, sizeof(double));
	x = x * (1.5 - xhalf * x * x);
	x = x * (1.5 - xhalf * x * x);
	x = x * (1.5 - xhalf * x * x);
	return (x * n);
}

/*
float	sqrt_approx_f32(float n)
{
	float	x;
	float	xhalf;
	int32_t	i;

	x = n;
	xhalf = 0.5f * x;
	memcpy(&i, &x, sizeof(int32_t));
	i = 0x5f375a86 - (i >> 1);
	memcpy(&x, &i, sizeof(float));
	x = x * (1.5f - xhalf * x * x);
	x = x * (1.5f - xhalf * x * x);
	x = x * (1.5f - xhalf * x * x);
	return (x * n);
}
*/
