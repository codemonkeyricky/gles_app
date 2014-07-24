#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "IL/il.h"

#include "renderer.hpp"

class Entity
{
public:
    Entity(Renderer * renderer);
    void draw() const;
    void setX();
    void setY();

private:
    void        textureLoad();
    int         m_posX;
    int         m_posY;
    unsigned int    m_width;
    unsigned int    m_height;
    float       m_rot;
    GLuint      m_texture;
    Renderer   *m_renderer;
};

#endif // #define ENTITY_HPP
