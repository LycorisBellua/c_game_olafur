#include "../headers/maths.h"

/*
    In the header:
    - PI

    - ABS(x)
    - NORMALIZE(x)
    - MIN(a, b)
    - MAX(a, b)
    - CLAMP(x, min, max)
    - CLAMP_MIN(x, min)
    - CLAMP_MAX(x, max)
    - IS_CLOSE_TO_ZERO(x)
    - DEG2RAD(degrees)
    - RAD2DEG(radians)
*/

static float factorial(const int n);
static float power(const float base, const int exponent);

float f_sin(const float radians)
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

float f_cos(const float radians)
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

float f_tan(const float radians)
{
    float cos = f_cos(radians);

    if (IS_CLOSE_TO_ZERO(cos))
        return 0.0f;

    return f_sin(radians) / cos;
}

void swap(int* a, int* b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
    return;
}

int float_equality(float a, float b)
{
    /* If different signs */
    if ((a < 0 && b > 0) || (a > 0 && b < 0))
        return 0;

    /* Remove decimal part */
    a -= (int)a;
    b -= (int)b;

    /* Check up to 7 digits after the floating point */
    a *= power(10, 7);
    b *= power(10, 7);
    return (int)a == (int)b;
}

float f_sqrt(float number)
{
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wstrict-aliasing"
    #pragma GCC diagnostic ignored "-Wuninitialized"
    float x = number;
    float xhalf = 0.5 * x;
    int i = *(int*)&x;
    i = 0x5f375a86 - (i >> 1);
    x = *(float*)&i;
    x = x * (1.5 - xhalf * x * x);
    x = x * (1.5 - xhalf * x * x);
    x = x * (1.5 - xhalf * x * x);
    return x * number;
    #pragma GCC diagnostic pop
}

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

