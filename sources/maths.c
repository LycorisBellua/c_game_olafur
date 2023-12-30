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
    float result = 1.0f;
    int i;
    for (i = 0; i < exponent; ++i)
        result *= base;
    return result;
}

static float fsin(const float radians)
{
    float result = 0.0f;
    int n;
    for (n = 0; n < 10; ++n)
        result += power(-1, n) * power(radians, 2 * n + 1) / factorial(2 * n + 1);
    return result;
}

static float fcos(const float radians)
{
    float result = 0.0f;
    int n;
    for (n = 0; n < 10; ++n)
        result += power(-1, n) * power(radians, 2 * n) / factorial(2 * n);
    return result;
}

void compose_transform_matrix(float* matrix, float scaleX, float scaleY, float scaleZ,
                            float rotateX, float rotateY, float rotateZ,
                            float translateX, float translateY, float translateZ)
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

        And as to why people seem confused online as to whether OpenGL is left 
        or right-handed by default, it's because "OpenGL ES" (for Embedded 
        Systems) and the deprecated fixed-point pipeline are both right-handed.
    */

    int i, j, k;

    /* Convert angles from degrees to radians */
    float radX = deg2rad(rotateX);
    float radY = deg2rad(rotateY);
    float radZ = deg2rad(rotateZ);

    /* Calculate sines and cosines */
    float sinX = fsin(radX);
    float cosX = fcos(radX);
    float sinY = fsin(radY);
    float cosY = fcos(radY);
    float sinZ = fsin(radZ);
    float cosZ = fcos(radZ);

    float matrix_rotateX[16];
    float matrix_rotateY[16];
    float matrix_rotateZ[16];
    float matrix_temp[16];

    /* Identity */
    matrix[0] = 1.0f;
    for (i = 1; i < 16; ++i)
        matrix[i] = !(i % 5);

    for (i = 0; i < 14; ++i)
    {
        matrix_rotateX[i] = matrix[i];
        matrix_rotateY[i] = matrix[i];
        matrix_rotateZ[i] = matrix[i];
    }

    /* Scale */
    matrix[0] *= scaleX;
    matrix[5] *= scaleY;
    matrix[10] *= scaleZ;

    /* Rotate around Z */
    matrix_rotateZ[0] = cosZ;
    matrix_rotateZ[1] = -sinZ;
    matrix_rotateZ[4] = sinZ;
    matrix_rotateZ[5] = cosZ;

    for (i = 0; i < 4; ++i)
    {
        for (j = 0; j < 4; ++j)
        {
            matrix_temp[i + 4 * j] = 0.0f;
            for (k = 0; k < 4; ++k)
                matrix_temp[i + 4 * j] += matrix[i + 4 * k] * matrix_rotateZ[k + 4 * j];
        }
    }
    for (i = 0; i < 16; ++i)
        matrix[i] = matrix_temp[i];

    /* Rotate around Y */
    matrix_rotateY[0] = cosY;
    matrix_rotateY[2] = -sinY;
    matrix_rotateY[8] = sinY;
    matrix_rotateY[10] = cosY;

    for (i = 0; i < 4; ++i)
    {
        for (j = 0; j < 4; ++j)
        {
            matrix_temp[i + 4 * j] = 0.0f;
            for (k = 0; k < 4; ++k)
                matrix_temp[i + 4 * j] += matrix[i + 4 * k] * matrix_rotateY[k + 4 * j];
        }
    }
    for (i = 0; i < 16; ++i)
        matrix[i] = matrix_temp[i];

    /* Rotate around X */
    matrix_rotateX[5] = cosX;
    matrix_rotateX[6] = sinX;
    matrix_rotateX[9] = -sinX;
    matrix_rotateX[10] = cosX;

    for (i = 0; i < 4; ++i)
    {
        for (j = 0; j < 4; ++j)
        {
            matrix_temp[i + 4 * j] = 0.0f;
            for (k = 0; k < 4; ++k)
                matrix_temp[i + 4 * j] += matrix[i + 4 * k] * matrix_rotateX[k + 4 * j];
        }
    }
    for (i = 0; i < 16; ++i)
        matrix[i] = matrix_temp[i];

    /* Translate */
    matrix[12] += translateX;
    matrix[13] += translateY;
    matrix[14] += translateZ;
    /* matrix[15] became 0 at some point and it needs to be 1 */
    matrix[15] = 1.0f;
    return;
}

