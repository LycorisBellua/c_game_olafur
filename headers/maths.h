#ifndef __MATHS_H__
#define __MATHS_H__

#define PI 3.1415926535f
#define PI_OVER_180 0.017453293f
#define PI_OVER_180_INVERSE 57.295779515f

#define   RAD_1 0.017453293f
#define  RAD_45 0.785398163f
#define  RAD_90 1.570796327f
#define RAD_180 PI
#define RAD_270 4.71238898f
#define RAD_360 6.283185307f

/*
    PI_OVER_180 = `PI/180`.
    PI_OVER_180_INVERSE = `180/PI`.

      RAD_1 = PI*2/360
     RAD_45 = PI/4
     RAD_90 = PI/2
    RAD_180 = PI
    RAD_270 = PI/2*3
    RAD_360 = PI*2
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
float f_sqrt(const float number);
void swap(int* a, int* b);
int float_equality(float a, float b);
float clamp_radians(const float angle);

#endif

