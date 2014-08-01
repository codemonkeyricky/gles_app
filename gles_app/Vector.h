#pragma once

#include "assert.h"

class Vector2f
{
public:
    union
    {
        float   value[2];
        struct { float x, y; };
		struct { float width, height; };
    };

    // Subscript operators.
    float & operator [](const size_t i) { assert(i < 2); return value[i]; }
    const float & operator [](const size_t i) const { assert(i < 2); return value[i]; }

    // Arithmetic operators.
    Vector2f operator +(const Vector2f &b) const { return Vector2f(x + b.x, y + b.y); }
    Vector2f operator -(const Vector2f &b) const { return Vector2f(x - b.x, y - b.y); }
    Vector2f operator *(const Vector2f &b) const { return Vector2f(x * b.x, y * b.y); }
    Vector2f operator /(const float &b) const { return Vector2f(x/b, y/b); }
    Vector2f operator *(const float &b) const { return Vector2f(x * b, y * b); }

    bool operator ==(const Vector2f &b) const { return (x == b.x) && (y == b.y); }
    bool operator !=(const Vector2f &b) const { return *this != b; }

    Vector2f& operator +=(const Vector2f &b) { *this = *this + b; return *this; }

public:
    // Constructors.
    explicit Vector2f() : x(0), y(0) { }
    explicit Vector2f(const float &nx, const float &ny) : x(nx), y(ny) {}

public:
	float length() const; 
	float lengthSquared() const; 
	float distance(const Vector2f &p1) const;
	float distanceSquared(const Vector2f &p1) const;
    float dot(const Vector2f &v2) const;
    Vector2f normalize() const;
};

Vector2f operator *(const float &s, const Vector2f &v); 
