#include <Windows.h> 

class Math
{
public:
    static const float PI;

public:
    static float radians(const float &degrees)
    {
        return degrees * (PI / 180.0f);
    }

    static float degrees(const float &radians)
    {
        return radians * (180.0f / PI);
    }

	static float clamp(const float &x, const float &minVal, const float &maxVal)
    {
        return max(min(x, maxVal), minVal);
    }

#if 0
    static float min(const float &x, const float &y)
    {
        return (x < y) ? x : y;
    }

    static float max(const float &x, const float &y)
    {
        return (x > y) ? x : y;
    }
#endif
    
    static float mix(float const & x, float const & y, float const & a)
    {
        return float(float(x) + a * float(y - x));
    }

    static float random();

};
