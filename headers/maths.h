#ifndef __MATHS_H__
#define __MATHS_H__

#define PI 3.1415926535
#define PI_OVER_180 0.017453293
#define PI_OVER_180_INVERSE 57.295779515
#define PI_OVER_4 0.785398163

/*
    PI_OVER_180 = `PI/180`.
    PI_OVER_180_INVERSE = `180/PI`.
    PI_OVER_4 = `PI/4` or `45° * PI_OVER_180`.
*/

#define ABS(x)              ((x) < 0 ? -(x) : (x))
#define NORMALIZE(x)        ((x) < 0 ? -1 : (x) > 0 ? 1 : 0)
#define MIN(a, b)           ((a) < (b) ? (a) : (b))
#define MAX(a, b)           ((a) > (b) ? (a) : (b))
#define CLAMP(x, min, max)  ((x) < (min) ? (min) : (x) > (max) ? (max) : (x))
#define CLAMP_MIN(x, min)   ((x) < (min) ? (min) : (x))
#define CLAMP_MAX(x, max)   ((x) > (max) ? (max) : (x))
#define IS_CLOSE_TO_ZERO(x) (ABS(x) < 1e-15)
#define DEG2RAD(degrees)    ((degrees) * PI_OVER_180)
#define RAD2DEG(radians)    ((radians) * PI_OVER_180_INVERSE)

float f_sin(const float radians);
float f_cos(const float radians);
float f_tan(const float radians);
void swap(int* a, int* b);
int float_equality(float a, float b);
float f_sqrt(float number);

#endif

