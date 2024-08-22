#ifndef MATH_H
#define MATH_H

#define PI 3.1415926535
/* #define PI_OVER_180 (PI/180) */
/* #define PI_OVER_180_INVERSE (180/PI) */
/* #define RAD_1 (PI*2/360) */
/* #define RAD_45 (PI/4) */
/* #define RAD_60 (PI/3) */
/* #define RAD_90 (PI/2) */
/* #define RAD_180 (PI) */
/* #define RAD_270 (PI/2*3) */
/* #define RAD_360 (PI*2) */
#define PI_OVER_180 0.017453293
#define PI_OVER_180_INVERSE 57.295779515
#define RAD_1 0.017453293
#define RAD_45 0.785398163
#define RAD_60 1.047197551
#define RAD_90 1.570796327
#define RAD_180 PI
#define RAD_270 4.71238898
#define RAD_360 6.283185307

int    abs(int n);
double f_abs(double n);
int    min(int a, int b);
double f_min(double a, double b);
int    max(int a, int b);
double f_max(double a, double b);
double f_floor(double n);
double f_ceil(double n);
double f_round(double n);
int    normalize(int n);
double f_normalize(double n);
int    clamp(int n, int min, int max);
double f_clamp(double n, double min, double max);
int    clamp_min(int n, int min);
double f_clamp_min(double n, double min);
int    clamp_max(int n, int max);
double f_clamp_max(double n, double max);
double clamp_rad(double rad);
double deg2rad(double deg);
double rad2deg(double rad);
double f_sin(double rad);
double f_cos(double rad);
double f_tan(double rad);
double f_pow(double base, int exponent);
double factorial(int n);
double f_sqrt(double n);
void   swap(int* a, int* b);
int    double_equality(double a, double b);
int	   rng_minmax(int *seed, int min, int max);

#endif
