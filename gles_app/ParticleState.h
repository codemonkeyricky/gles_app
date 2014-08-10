#pragma once

#include "Vector.h"

class Particle;

class ParticleState
{
public:
    enum ParticleType
    {
        kNone = 0,
        kEnemy,
        kBullet,
        kIgnoreGravity
    };

public:
    Vector2f        m_velocity;
    ParticleType    m_type;
    float           m_lengthMultiplier;

public:
    ParticleState();
    ParticleState(
        const Vector2f &velocity,
        ParticleType    type,
        float           lengthMultiplier = 1.0f
        );

    ParticleState   getRandom(float minVel, float maxVel);
    void            updateParticle(Particle &particle);
};
