#pragma once

#include "GLES2/gl2.h"
#include "EGL/egl.h"
#include "Vector.h"

class Surface;

class Texture
{
protected:
    Dimension2f m_textureSize;
    Dimension2f m_surfaceSize;
    GLuint      m_textureID;

public:
    Texture(const Surface& newSurface);
    ~Texture();

    Dimension2f getSize() const;
    Dimension2f getSurfaceSize() const;

    friend class Surface;
    friend class Renderer;
};
