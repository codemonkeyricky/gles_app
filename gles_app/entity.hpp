#pragma once

#include "IL/il.h"

#include "renderer.hpp"

class Entity
{
public:
    enum Kind
    {
        kDontCare = 0,
        kBullet,
        kEnemy,
        kBlackHole
    };

protected:
    GLuint      m_texture;
//    Renderer   *m_renderer;
    Vector2f    m_position;
    Vector2f    m_dimension;
    Vector2f    m_velocity;
    float       m_orientation;
    float       m_radius;
    bool        m_isExpired;
    Kind        m_kind;

public:
	Entity(); 
    Entity(Renderer * renderer);
    virtual ~Entity();

    Vector2f getSize() const;
    virtual void update() = 0;
    virtual void draw();

    Vector2f    getPosition() const;
    Vector2f    getVelocity() const;
    void        setVelocity(const Vector2f &nv);
    float       getRadius() const;
    bool        isExpired() const;
    Kind        getKind() const;

    void        setExpired();

private:
    void        textureLoad();
};
