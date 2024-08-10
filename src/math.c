#include "cimmerian.h"

static double is_close_to_zero(const double n);
static double factorial(const int n);
static double power(const double base, const int exponent);

int abs(const int n)
{
    if (n < 0)
        return -n;
    return n;
}

double f_abs(const double n)
{
    if (n < 0)
        return -n;
    return n;
}

int min(const int a, const int b)
{
    if (a < b)
        return a;
    return b;
}

int max(const int a, const int b)
{
    if (a > b)
        return a;
    return b;
}

int normalize(const int n)
{
    if (n < 0)
        return -1;
    else if (n > 0)
        return 1;
    return 0;
}

int clamp(const int n, const int min, const int max)
{
    if (n < min)
        return min;
    else if (n > max)
        return max;
    return n;
}

int clamp_min(const int n, const int min)
{
    if (n < min)
        return min;
    return n;
}

int clamp_max(const int n, const int max)
{
    if (n > max)
        return max;
    return n;
}

double deg2rad(const double deg)
{
    return deg * PI_OVER_180;
}

double rad2deg(const double rad)
{
    return rad * PI_OVER_180_INVERSE;
}

double f_sin(const double rad)
{
    int n;
    double result;
    double fact;

    n = 0;
    result = 0.0f;
    while (n < 10)
    {
        fact = factorial(2 * n + 1);
        if (!is_close_to_zero(fact))
            result += power(-1, n) * power(rad, 2 * n + 1) / fact;
        ++n;
    }
    return result;
}

double f_cos(const double rad)
{
    int n;
    double result;
    double fact;

    n = 0;
    result = 0.0f;
    while (n < 10)
    {
        fact = factorial(2 * n);
        if (!is_close_to_zero(fact))
            result += power(-1, n) * power(rad, 2 * n) / fact;
        ++n;
    }
    return result;
}

double f_tan(const double rad)
{
    double cos;

    cos = f_cos(rad);
    if (is_close_to_zero(cos))
        return 0.0f;
    return f_sin(rad) / cos;
}

double f_sqrt(const double n)
{
    double x;
    double xhalf;
    int i;

    x = n;
    xhalf = 0.5 * x;
    i = *(int*)&x;
    i = 0x5f375a86 - (i >> 1);
    x = *(double*)&i;
    x = x * (1.5 - xhalf * x * x);
    x = x * (1.5 - xhalf * x * x);
    x = x * (1.5 - xhalf * x * x);
    return x * n;
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

    /* Check up to 7 digits after the doubleing point */
    a *= power(10, 7);
    b *= power(10, 7);
    return (int)a == (int)b;
}

double clamp_rad(const double rad)
{
    if (rad < 0)
        return rad + RAD_360;
    else if (rad > RAD_360)
        return rad - RAD_360;
    return rad;
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

static double is_close_to_zero(const double n)
{
    return f_abs(n) < 1e-15;
}

static double factorial(const int n)
{
    if (n < 2)
        return 1.0f;
    return n * factorial(n - 1);
}

static double power(const double base, const int exponent)
{
    int i;
    double result;

    i = 0;
    result = 1.0f;
    while (i < exponent)
    {
        result *= base;
        ++i;
    }
    return result;
}
