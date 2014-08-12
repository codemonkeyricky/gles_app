#pragma once

#include "Vector.h"

class MathUtil
{
public:
    static Vector2f fromPolar(float angle, float magnitude);

    static float catmullRom(const float value1, const float value2, const float value3, const float value4, float amount);
    static Vector2f catmullRom(const Vector2f& value1, const Vector2f& value2, const Vector2f& value3, const Vector2f& value4, float amount);
};

