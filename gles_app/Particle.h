#pragma once

#include "Texture.h"
#include "ParticleState.h"

class Particle
{
public:
    ParticleState   m_state;
    Color4f         m_color;
    Vector2f        m_position;
    Vector2f        m_scale;
    Texture        *m_texture;
    float           m_orientation;
    float           m_duration;
    float           m_percentLife;

public:
    Particle()
        : m_scale(1,1),
          m_percentLife(1.0f) { }
};
