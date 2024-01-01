#include <stdio.h>
#include "../headers/maths.h"

#define PI 3.1416

static float deg2rad(const float degrees)
{
    return degrees * PI/180;
}

/*
static float rad2deg(const float radians)
{
    return radians * 180/PI;
}
*/

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

static float f_abs(const float value)
{
    return (value < 0) ? -value : value;
}

static float f_sin(const float radians)
{
    int n;
    float result = 0.0f;
    float fact;
    for (n = 0; n < 10; ++n)
    {
        fact = factorial(2 * n + 1);
        /* Check if factorial is close to zero to avoid division by zero */
        if (f_abs(fact) < 1e-15)
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
        if (f_abs(fact) < 1e-15)
            result += 0.0f;
        else
            result += power(-1, n) * power(radians, 2 * n) / fact;
    }
    return result;
}

static float f_tan(const float radians)
{
    float sine = f_sin(radians);
    float cosine = f_cos(radians);

    if (f_abs(cosine) < 1e-15)
        return 0.0f;

    return sine / cosine;
}

void compose_transform_matrix(float* matrix, 
    const float scale_x, const float scale_y, const float scale_z,
    const float rotate_x, const float rotate_y, const float rotate_z,
    const float translate_x, const float translate_y, const float translate_z)
{
    /*
        By default, OpenGL reads matrices in column-major order, and has a 
        left-handed coordinate system. Left or right-handed is about how the Z 
        axis works: is what comes out of the screen the negative part or the 
        positive part? By default, the Z axis starts negative and becomes 
        positive the further into the 3D world we go, meaning the coordinate 
        system is left-handed. This also means that the Z rotation is counter 
        clockwise.

        This is vital to know for two reasons:
        - The Z rotation matrix that can be found online is likely to be for a 
        clockwise type of rotation. I've changed my Z rotation matrix to match.
        - In a left-handed system, the rotation matrices are to be multiplied 
        in a ZYX order, not XYZ. The order matters because matrix 
        multiplication is not commutative, meaning that A*B != B*A.
        - View Matrix: In the end, I've put the yaw last, because it's the 
        only axis that I change at runtime and this rotation would have been 
        funky otherwise.

        And as to why people seem confused online as to whether OpenGL is left 
        or right-handed by default, it's because "OpenGL ES" (for Embedded 
        Systems) and the deprecated fixed-point pipeline are both right-handed.
    */

    int i, j, k;

    /* Convert angles from degrees to radians */
    const float rad_x = deg2rad(rotate_x);
    const float rad_y = deg2rad(rotate_y);
    const float rad_z = deg2rad(rotate_z);

    /* Calculate sines and cosines */
    const float sin_x = f_sin(rad_x);
    const float cos_x = f_cos(rad_x);
    const float sin_y = f_sin(rad_y);
    const float cos_y = f_cos(rad_y);
    const float sin_z = f_sin(rad_z);
    const float cos_z = f_cos(rad_z);

    float matrix_rotate_x[16];
    float matrix_rotate_y[16];
    float matrix_rotate_z[16];
    float matrix_temp[16];

    /* Identity */
    matrix[0] = 1.0f;
    for (i = 1; i < 16; ++i)
        matrix[i] = !(i % 5);

    for (i = 0; i < 14; ++i)
    {
        matrix_rotate_x[i] = matrix[i];
        matrix_rotate_y[i] = matrix[i];
        matrix_rotate_z[i] = matrix[i];
    }

    /* Scale */
    matrix[0] *= scale_x;
    matrix[5] *= scale_y;
    matrix[10] *= scale_z;

    /* Rotate around Z */
    matrix_rotate_z[0] = cos_z;
    matrix_rotate_z[1] = -sin_z;
    matrix_rotate_z[4] = sin_z;
    matrix_rotate_z[5] = cos_z;

    for (i = 0; i < 4; ++i)
    {
        for (j = 0; j < 4; ++j)
        {
            matrix_temp[i + 4 * j] = 0.0f;
            for (k = 0; k < 4; ++k)
                matrix_temp[i + 4 * j] += matrix[i + 4 * k] 
                    * matrix_rotate_z[k + 4 * j];
        }
    }
    for (i = 0; i < 16; ++i)
        matrix[i] = matrix_temp[i];

    /* Rotate around X */
    matrix_rotate_x[5] = cos_x;
    matrix_rotate_x[6] = sin_x;
    matrix_rotate_x[9] = -sin_x;
    matrix_rotate_x[10] = cos_x;

    for (i = 0; i < 4; ++i)
    {
        for (j = 0; j < 4; ++j)
        {
            matrix_temp[i + 4 * j] = 0.0f;
            for (k = 0; k < 4; ++k)
                matrix_temp[i + 4 * j] += matrix[i + 4 * k] 
                    * matrix_rotate_x[k + 4 * j];
        }
    }
    for (i = 0; i < 16; ++i)
        matrix[i] = matrix_temp[i];

    /* Rotate around Y */
    matrix_rotate_y[0] = cos_y;
    matrix_rotate_y[2] = -sin_y;
    matrix_rotate_y[8] = sin_y;
    matrix_rotate_y[10] = cos_y;

    for (i = 0; i < 4; ++i)
    {
        for (j = 0; j < 4; ++j)
        {
            matrix_temp[i + 4 * j] = 0.0f;
            for (k = 0; k < 4; ++k)
                matrix_temp[i + 4 * j] += matrix[i + 4 * k] 
                    * matrix_rotate_y[k + 4 * j];
        }
    }
    for (i = 0; i < 16; ++i)
        matrix[i] = matrix_temp[i];

    /* Translate */
    matrix[12] = translate_x;
    matrix[13] = translate_y;
    matrix[14] = translate_z;
    matrix[15] = 1.0f;
    return;
}

void compose_perspective_projection_matrix(float* matrix, 
    const float fov, const float aspect_ratio, 
    const float near_clip, const float far_clip)
{
    /* Clipping plane distances */
    const float tan_half_fov = f_tan(deg2rad(fov/2));
    const float right = near_clip * tan_half_fov;
    const float left = -right;
    const float top = near_clip * tan_half_fov / aspect_ratio;
    const float bottom = -top;

    matrix[0] = (2 * near_clip) / (right - left);
    matrix[1] = 0.0f;
    matrix[2] = (right + left) / (right - left);
    matrix[3] = 0.0f;

    matrix[4] = 0.0f;
    matrix[5] = (2 * near_clip) / (top - bottom);
    matrix[6] = (top + bottom) / (top - bottom);
    matrix[7] = 0.0f;

    matrix[8] = 0.0f;
    matrix[9] = 0.0f;
    matrix[10] = -(far_clip + near_clip) / (far_clip - near_clip);
    matrix[11] = -2 * far_clip * near_clip / (far_clip - near_clip);

    matrix[12] = 0.0f;
    matrix[13] = 0.0f;
    matrix[14] = -1.0f;
    matrix[15] = 0.0f;

    return;
}

