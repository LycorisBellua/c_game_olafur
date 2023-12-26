#include "../headers/maths.h"

#define PI 3.1416

float deg2rad(const float degrees)
{
    return degrees * PI/180;
}

float rad2deg(const float radians)
{
    return radians * 180/PI;
}

