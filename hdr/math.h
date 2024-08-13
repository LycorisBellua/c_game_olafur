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

int    abs(int n);
double f_abs(double n);
int    min(int a, int b);
int    f_min(double a, double b);
int    max(int a, int b);
int    f_max(double a, double b);
int    normalize(int n);
int    clamp(int n, int min, int max);
int    clamp_min(int n, int min);
int    clamp_max(int n, int max);
double deg2rad(double deg);
double rad2deg(double rad);
double f_sin(double rad);
double f_cos(double rad);
double f_tan(double rad);
double f_sqrt(double n);
void   swap(int* a, int* b);
int    double_equality(double a, double b);
double clamp_rad(double rad);
int	   rng_minmax(int *seed, int min, int max);

#endif
