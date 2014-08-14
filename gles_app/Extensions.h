#pragma once

#include "Vector.h"

class Extensions
{
public:
    static float    toAngle(const Vector2f &vector);
    static Vector2f scaleTo(const Vector2f &vector, float length);
    static Vector2f toPoint(const Vector2f &vector);
    static float    nextFloat(float minValue, float maxValue);
    static Vector2f nextVector2(float minLength, float maxLength);

    static Color4f  colorLerp(const Color4f &a, const Color4f &b, float alpha);

    static void Extensions::drawLine(
         Vector2f& start,
        const Vector2f& end,
         Color4f& color,
        float thickness
        );
};
