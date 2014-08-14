#include "Extensions.h"
#include "Math.h"
#include "Vector.h"
#include "Extensions.h"
#include <math.h>
#include "Renderer.h"
#include "Art.h"

float Extensions::toAngle(
    const Vector2f &vector
    )
{
    return (float) atan2f(vector.y, vector.x);
}


Vector2f Extensions::scaleTo(
    const Vector2f &vector,
    float length
    )
{
    return vector * (length / vector.length());
}


Vector2f Extensions::toPoint(
    const Vector2f &vector
    )
{
    return vector;
}


float Extensions::nextFloat(
    float minValue,
    float maxValue
    )
{
    return (float)Math::random() * (maxValue - minValue) + minValue;

}


Vector2f Extensions::nextVector2(
    float minLength,
    float maxLength
    )
{
    float theta = Math::random() * 2.0f * Math::PI;
    float length = nextFloat(minLength, maxLength);

    return Vector2f(length * (float) cosf(theta), length * (float) sinf(theta));
}


Color4f Extensions::colorLerp(
    const Color4f &a,
    const Color4f &b,
    float alpha
    )
{
    return Color4f(Math::mix(a.r, b.r, alpha),
                   Math::mix(a.g, b.g, alpha),
                   Math::mix(a.b, b.b, alpha),
                   Math::mix(a.a, b.a, alpha));
}


void Extensions::drawLine(
    Vector2f& start,
    const Vector2f& end,
    Color4f& color,
    float thickness
    )
{
    Vector2f delta = end - start;


    Renderer::getInstance()->draw(
        start,
        Vector2f(delta.length(), thickness),
        toAngle(delta),
        Art::getInstance()->getPixel(),
        color);
}
