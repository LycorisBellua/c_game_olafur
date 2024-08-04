#ifndef MATH_H
#define MATH_H

#define PI 3.1415926535f
/* #define PI_OVER_180 (PI/180) */
/* #define PI_OVER_180_INVERSE (180/PI) */
/* #define RAD_1 (PI*2/360) */
/* #define RAD_45 (PI/4) */
/* #define RAD_90 (PI/2) */
/* #define RAD_180 (PI) */
/* #define RAD_270 (PI/2*3) */
/* #define RAD_360 (PI*2) */
#define PI_OVER_180 0.017453293f
#define PI_OVER_180_INVERSE 57.295779515f
#define RAD_1 0.017453293f
#define RAD_45 0.785398163f
#define RAD_90 1.570796327f
#define RAD_180 PI
#define RAD_270 4.71238898f
#define RAD_360 6.283185307f

int   abs(const int n);
float f_abs(const float n);
int   min(const int a, const int b);
int   max(const int a, const int b);
int   normalize(const int n);
int   clamp(const int n, const int min, const int max);
int   clamp_min(const int n, const int min);
int   clamp_max(const int n, const int max);
float deg2rad(const float deg);
float rad2deg(const float rad);
float f_sin(const float rad);
float f_cos(const float rad);
float f_tan(const float rad);
float f_sqrt(const float n);
void  swap(int* a, int* b);
int   float_equality(float a, float b);
float clamp_rad(const float rad);
int	  rng_minmax(int *seed, int min, int max);

#endif
