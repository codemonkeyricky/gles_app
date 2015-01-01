#include "Matrix.h"

Vector2f operator *(const Vector2f &ct, const Matrix2x2f &m) { return Vector2f(ct.dot(m[0]), ct.dot(m[1])); }
Vector2f operator *(const Matrix2x2f &m, const Vector2f &rt) { return rt * m.transpose(); } 