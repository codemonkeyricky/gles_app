#pragma once

#include "Vector.h"

class ColorUtil
{
public:
    static Vector3f ColorToHSV(const Color4f &color);
    static Color4f HSVToColor(const Vector3f &hsv);
    static Color4f HSVToColor(float h, float s, float v);
};
