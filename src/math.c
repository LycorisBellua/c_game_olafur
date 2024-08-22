#include "cimmerian.h"

static double is_close_to_zero(double n);

int abs(int n)
{
    if (n < 0)
        return -n;
    return n;
}

double f_abs(double n)
{
    if (n < 0)
        return -n;
    return n;
}

int min(int a, int b)
{
    if (a < b)
        return a;
    return b;
}

double f_min(double a, double b)
{
    if (a < b)
        return a;
    return b;
}

int max(int a, int b)
{
    if (a > b)
        return a;
    return b;
}

double f_max(double a, double b)
{
    if (a > b)
        return a;
    return b;
}

double f_floor(double n)
{
    return (int)n;
}

double f_ceil(double n)
{
    return (int)n + 1;
}

double f_round(double n)
{
    if (n - (int)n < 0.5)
        return f_floor(n);
    return f_ceil(n);
}

int normalize(int n)
{
    if (n < 0)
        return -1;
    else if (n > 0)
        return 1;
    return 0;
}

double f_normalize(double n)
{
    if (n < 0)
        return -1;
    else if (n > 0)
        return 1;
    return 0;
}

int clamp(int n, int min, int max)
{
    if (n < min)
        return min;
    else if (n > max)
        return max;
    return n;
}

double f_clamp(double n, double min, double max)
{
    if (n < min)
        return min;
    else if (n > max)
        return max;
    return n;
}

int clamp_min(int n, int min)
{
    if (n < min)
        return min;
    return n;
}

double f_clamp_min(double n, double min)
{
    if (n < min)
        return min;
    return n;
}

int clamp_max(int n, int max)
{
    if (n > max)
        return max;
    return n;
}

double f_clamp_max(double n, double max)
{
    if (n > max)
        return max;
    return n;
}

double clamp_rad(double rad)
{
    if (rad < 0)
        return rad + RAD_360;
    else if (rad > RAD_360)
        return rad - RAD_360;
    return rad;
}

double deg2rad(double deg)
{
    return deg * PI_OVER_180;
}

double rad2deg(double rad)
{
    return rad * PI_OVER_180_INVERSE;
}

double f_sin(double rad)
{
    int n;
    double result;
    double fact;

    n = 0;
    result = 0.0;
    while (n < 10)
    {
        fact = factorial(2 * n + 1);
        if (!is_close_to_zero(fact))
            result += f_pow(-1, n) * f_pow(rad, 2 * n + 1) / fact;
        ++n;
    }
    return result;
}

double f_cos(double rad)
{
    int n;
    double result;
    double fact;

    n = 0;
    result = 0.0;
    while (n < 10)
    {
        fact = factorial(2 * n);
        if (!is_close_to_zero(fact))
            result += f_pow(-1, n) * f_pow(rad, 2 * n) / fact;
        ++n;
    }
    return result;
}

double f_tan(double rad)
{
    double cos;

    cos = f_cos(rad);
    if (is_close_to_zero(cos))
        return 0.0;
    return f_sin(rad) / cos;
}

double f_pow(double base, int exponent)
{
    int i;
    double result;

    i = 0;
    result = 1.0;
    while (i < exponent)
    {
        result *= base;
        ++i;
    }
    return result;
}

double factorial(int n)
{
    if (n < 2)
        return 1.0;
    return n * factorial(n - 1);
}

double f_sqrt(double n)
{
    double x;
    double xhalf;
    int64_t i;

    x = n;
    xhalf = 0.5 * x;
    i = *(int64_t*)&x;
    i = 0x5fe6ec85e7de30da - (i >> 1);
    x = *(double*)&i;
    x = x * (1.5 - xhalf * x * x);
    x = x * (1.5 - xhalf * x * x);
    x = x * (1.5 - xhalf * x * x);
    return x * n;

/*
    SQRT FLOAT VERSION:

    float x;
    float xhalf;
    int32_t i;

    x = n;
    xhalf = 0.5f * x;
    i = *(int32_t*)&x;
    i = 0x5f375a86 - (i >> 1);
    x = *(float*)&i;
    x = x * (1.5f - xhalf * x * x);
    x = x * (1.5f - xhalf * x * x);
    x = x * (1.5f - xhalf * x * x);
    return x * n;
*/
}

void swap(int* a, int* b)
{
    int tmp;

    tmp = *a;
    *a = *b;
    *b = tmp;
    return;
}

int double_equality(double a, double b)
{
    /* If different signs */
    if ((a < 0 && b > 0) || (a > 0 && b < 0))
        return 0;

    /* Remove decimal part */
    a -= (int)a;
    b -= (int)b;

    /* Check up to 7 digits after the floating point */
    a *= f_pow(10, 7);
    b *= f_pow(10, 7);
    return (int)a == (int)b;
}

/* 32-bit Xorshift pseudo-RNG */
int	rng_minmax(int *seed, int min, int max)
{
    unsigned int random;

    if (!*seed)
        *seed = 1;
    random = *seed;
    random ^= random << 13;
    random ^= random >> 17;
    random ^= random << 5;
    *seed = random;
    return random % (max - min + 1) + min;
}

static double is_close_to_zero(double n)
{
    return f_abs(n) < 1e-15;
}
