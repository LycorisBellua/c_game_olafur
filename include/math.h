#ifndef MATH_H
# define MATH_H

# define PI 3.1415926535
/* # define PI_OVER_180 (PI / 180) */
/* # define PI_OVER_180_INVERSE (180 / PI) */
/* # define RAD_1 (PI * 2 / 360) */
/* # define RAD_45 (PI / 4) */
/* # define RAD_60 (PI / 3) */
/* # define RAD_90 (PI / 2) */
/* # define RAD_180 (PI) */
/* # define RAD_270 (PI / 2 * 3) */
/* # define RAD_360 (PI * 2) */
# define PI_OVER_180 0.017453293
# define PI_OVER_180_INVERSE 57.295779515
# define RAD_1 0.017453293
# define RAD_45 0.785398163
# define RAD_60 1.047197551
# define RAD_90 1.570796327
# define RAD_180 PI
# define RAD_270 4.71238898
# define RAD_360 6.283185307

/* Absolute ----------------------------------------------------------------- */

int		abs(int n);
double	abs_f(double n);

/* Angles ------------------------------------------------------------------- */

double	deg2rad(double deg);
double	rad2deg(double rad);
double	get_angle_from_dir(double dir_x, double dir_y);

/* Clamp -------------------------------------------------------------------- */

int		clamp(int n, int min, int max);
double	clamp_f(double n, double min, double max);
double	clamp_rad(double rad);

/* Clamp max ---------------------------------------------------------------- */

int		clamp_max(int n, int max);
double	clamp_max_f(double n, double max);

/* Clamp min ---------------------------------------------------------------- */

int		clamp_min(int n, int min);
double	clamp_min_f(double n, double min);

/* Distance ----------------------------------------------------------------- */

double	get_dist_euclidean(double ax, double ay, double bx, double by);
double	get_squared_dist_euclidean(double ax, double ay, double bx, double by);
double	get_dist_manhattan(double ax, double ay, double bx, double by);

/* Factorial ---------------------------------------------------------------- */

double	factorial(int n);

/* Float -------------------------------------------------------------------- */

double	floor_f(double n);
double	ceil_f(double n);
double	round_f(double n);
int		double_equality(double a, double b);

/* Max ---------------------------------------------------------------------- */

int		max(int a, int b);
double	max_f(double a, double b);

/* Min ---------------------------------------------------------------------- */

int		min(int a, int b);
double	min_f(double a, double b);

/* Normalize */

int		norm(int n);
double	norm_f(double n);

/* Power -------------------------------------------------------------------- */

double	pow_f(double base, int exponent);

/* RNG ---------------------------------------------------------------------- */

int		rng_minmax(int *seed, int min, int max);

/* Sqrt --------------------------------------------------------------------- */

double	sqrt_f(double n);
float	sqrt_f32(float n);

/* Swap --------------------------------------------------------------------- */

void	swap(int *a, int *b);

/* Trigonometry ------------------------------------------------------------- */

double	cos_f(double rad);
double	sin_f(double rad);
double	tan_f(double rad);

#endif
