#include "Texture.h"
#include "Surface.h"

Texture::Texture(const Surface &newSurface)
{
    // Remember size.
    m_textureSize = newSurface.m_size;
    m_surfaceSize = newSurface.m_size;

    // Create texture.
    glGenTextures(1, &m_textureID);
    assert(m_textureID != NULL);

    // Bind texture.
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Configure texture.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 (GLint) GL_RGBA,
                 (GLsizei) newSurface.m_size.width,
                 (GLsizei) newSurface.m_size.height,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 newSurface.m_ptr);

    // Unbind.
    glBindTexture(GL_TEXTURE_2D, 0);
}


Texture::~Texture()
{
    glDeleteTextures(1, &m_textureID);
}


Dimension2f Texture::getSize() const
{
    return m_textureSize;
}

Dimension2f Texture::getSurfaceSize() const
{
    return m_surfaceSize;
}
