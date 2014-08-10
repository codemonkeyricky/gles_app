#include <stdint.h>
#include "ColorUtil.h"
#include "Math.h"

Vector3f ColorUtil::ColorToHSV(
    const Color4f& color
    )
{
    Vector3f c(color.r, color.g, color.b);
    float v = max(c.x, max(c.y, c.z));
    float chroma = v - min(c.x, min(c.y, c.z));

    if (chroma != 0.0f)
    {
        float s = chroma / v;

        if (c.x >= c.y && c.y >= c.z)
        {
            float h = (c.y - c.z) / chroma;

            if (h < 0)
            {
                h += 6;
            }

            return Vector3f(h, s, v);
        }
        else if (c.y >= c.z && c.y >= c.x)
        {
            return Vector3f((c.z - c.x) / chroma + 2, s, v);
        }

        return Vector3f((c.x - c.y) / chroma + 4, s, v);
    }

    return Vector3f(0, 0, v);
}


Color4f ColorUtil::HSVToColor(const Vector3f& hsv)
{
    return HSVToColor(hsv.x, hsv.y, hsv.z);
}


Color4f ColorUtil::HSVToColor(float h, float s, float v)
{
    if (h == 0 && s == 0)
    {
        return Color4f(v, v, v, 1.0f);
    }

    float c = s * v;
    float x = c * (1 - abs(int32_t(h) % 2 - 1));
    float m = v - c;

    if (h < 1) return Color4f(c + m, x + m, m, 1.0f);
    else if (h < 2) return Color4f(x + m, c + m, m, 1.0f);
    else if (h < 3) return Color4f(m, c + m, x + m, 1.0f);
    else if (h < 4) return Color4f(m, x + m, c + m, 1.0f);
    else if (h < 5) return Color4f(x + m, m, c + m, 1.0f);
    else return Color4f(c + m, m, x + m, 1.0f);
}
