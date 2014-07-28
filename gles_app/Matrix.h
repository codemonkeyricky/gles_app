#include "assert.h"
#include "Vector.h"

class Matrix2x2f
{
public:
    Vector2f    value[2];

public:
    Vector2f & operator [] (const size_t i) { assert(i < 2); return value[i]; }
    const Vector2f & operator [] (const size_t i) const { assert(i < 2); return value[i]; }

public:
    Matrix2x2f operator +(const Matrix2x2f b) const
    {
        return Matrix2x2f(value[0] + b[0], value[1] + b[1]);
    }

public:
    explicit Matrix2x2f(const Vector2f &c0, const Vector2f &c1)
    {
        value[0] = c0;
        value[1] = c1;
    }

	Matrix2x2f transpose() const
	{
		return Matrix2x2f(Vector2f(value[0].x, value[1].x), 
			              Vector2f(value[0].y, value[1].y));
	}
};

Vector2f operator *(const Vector2f &ct, const Matrix2x2f &m);
Vector2f operator *(const Matrix2x2f &m, const Vector2f &ct);
