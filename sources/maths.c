#include "../headers/maths.h"

/*
    In the header:
    - MIN(a, b)
    - MAX(a, b)
    - CLAMP(x, min, max)
*/

#define PI 3.1416

#define ABS(x) (x < 0 ? -x : x)
#define IS_CLOSE_TO_ZERO(x) (ABS(x) < 1e-15)
#define DEG2RAD(degrees) (degrees * PI/180)
#define RAD2DEG(radians) (radians * 180/PI)

static float factorial(const int n);
static float power(const float base, const int exponent);
static float f_sin(const float radians);
static float f_cos(const float radians);
static float f_tan(const float radians);
static float clamp_euler_angle(float angle);

static float factorial(const int n)
{
    if (n == 0 || n == 1)
        return 1.0f;
    else
        return n * factorial(n - 1);
}

static float power(const float base, const int exponent)
{
    int i;
    float result = 1.0f;
    for (i = 0; i < exponent; ++i)
        result *= base;
    return result;
}

static float f_sin(const float radians)
{
    int n;
    float result = 0.0f;
    float fact;
    for (n = 0; n < 10; ++n)
    {
        fact = factorial(2 * n + 1);
        if (IS_CLOSE_TO_ZERO(fact))
            result += 0.0f;
        else
            result += power(-1, n) * power(radians, 2 * n + 1) / fact;
    }
    return result;
}

static float f_cos(const float radians)
{
    int n;
    float result = 0.0f;
    float fact;
    for (n = 0; n < 10; ++n)
    {
        fact = factorial(2 * n);
        if (IS_CLOSE_TO_ZERO(fact))
            result += 0.0f;
        else
            result += power(-1, n) * power(radians, 2 * n) / fact;
    }
    return result;
}

static float f_tan(const float radians)
{
    float cos = f_cos(radians);

    if (IS_CLOSE_TO_ZERO(cos))
        return 0.0f;

    return f_sin(radians) / cos;
}

static float clamp_euler_angle(float angle)
{
    if (angle < -360.0f)
        angle += 360.0f;
    else if (angle > 360.0f)
        angle -= 360.0f;
    return angle;
}

