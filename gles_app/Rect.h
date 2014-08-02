#pragma once

#include "Vector.h"

class Rectf
{
public:
    Vector2f    location;
    Vector2f    size;

    bool operator ==(const Rectf &b) const { return (location == b.location) && (size == b.size); }
    bool operator !=(const Rectf &b) const { return !(*this == b); }

public:

    Rectf() { }
    Rectf(float nx, float ny, float nw, float nh) : location(nx, ny), size(nw, nh) {}

    bool contains(const Vector2f newPt) const
    {
        if(   newPt.x >= location.x
           && newPt.y >= location.y
           && newPt.x < (location.x + size.width)
           && newPt.y < (location.y + size.height))
        {
            return true;
        }

        return false;
    }
};
