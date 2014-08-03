#pragma once

#include <string>
#include "Vector.h"

class Surface
{
protected:
    Dimension2f     m_size;
    unsigned short  m_bytesPerRow;
    char           *m_ptr;

    static bool     m_initialized;

public:
    Surface &operator=(const Surface &origSurface);

public:
    Surface(const std::string& file);
    Surface(const Dimension2f &newSize);
    Surface(const Surface &origSurface);
    ~Surface();

    Dimension2f getSize() const;

    friend class Texture;
};
