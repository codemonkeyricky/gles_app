#pragma once

#include <vector>
#include "assert.h"

#include "GLES2/gl2.h"
#include "EGL/egl.h"


class Vector2f
{
public:
    union
    {
        float   value[2];
        struct { float x, y; };
    };

    // Subscript operators.
    float & operator [](const size_t i) { assert(i < 2); return value[i]; }
    const float & operator [](const size_t i) const { assert(i < 2); return value[i]; }

    // Arithmetic operators.
    Vector2f operator +(const Vector2f &b) const { return Vector2f(x + b.x, y + b.y); }
    Vector2f operator -(const Vector2f &b) const { return Vector2f(x - b.x, y - b.y); }
    Vector2f operator *(const Vector2f &b) const { return Vector2f(x * b.x, y * b.y); }

public:
    // Constructors.
    explicit Vector2f() : x(0), y(0) { }
//    explicit Vector2f(const float& n) : x(n), y(n) { }
    explicit Vector2f(const float &nx, const float &ny) : x(nx), y(ny) {}

    float dot(const Vector2f &v2) const { return x * v2.x + y * v2.y; }
};


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
};

typedef struct
{
    GLuint                      texture;
    std::vector<float>          texCoords;
    std::vector<unsigned short> indices;
    std::vector<Vector2f>       vertices;
    std::vector<float>          colors;

} sEntityAttributes;


class Renderer
{
public:
    Renderer();
    virtual ~Renderer();

    void entityAdd();
    void entityRemove();

    void render();
    void draw(
        int             x,
        int             y,
        unsigned int    width,
        unsigned int    height,
        float           rot,
        GLuint          texture
    );


private:
    EGLDisplay  m_sEGLDisplay;
    EGLContext  m_sEGLContext;
    EGLSurface  m_sEGLSurface;
    HWND        m_hWindow;
    HDC         m_hDisplay;
    GLuint      m_uiProgram, m_uiFragShader, m_uiVertShader;
    GLuint      m_textureId;

    std::vector<sEntityAttributes>  m_entityAttributes;
};
