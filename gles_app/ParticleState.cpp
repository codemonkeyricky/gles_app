#include "ParticleState.h"
#include "Extensions.h"
#include "Particle.h"
#include "Math.h"
#include "constants.hpp"
#include <math.h>
#include <list>
#include "BlackHole.h"
#include "EntityManager.h"


ParticleState::ParticleState()
    : m_velocity(Vector2f(0,0)),
      m_type(kNone),
      m_lengthMultiplier(1.0f)
{
}


ParticleState::ParticleState(
    const Vector2f &velocity,
    ParticleType    type,
    float           lengthMultiplier
    ) : m_velocity(velocity),
        m_type(type),
        m_lengthMultiplier(lengthMultiplier)
{

}


ParticleState ParticleState::getRandom(
    float   minVel,
    float   maxVel
    )
{
    return ParticleState(Extensions::nextVector2(minVel, maxVel), kNone, 1);
}


void ParticleState::updateParticle(
    Particle &particle
    )
{
    Vector2f vel    = particle.m_state.m_velocity;
    float speed     = vel.length();

    particle.m_position += vel;

    float alpha = min(1.0f, min(particle.m_percentLife * 2, speed * 1.0f));
    alpha       *= alpha;

    particle.m_color.a = alpha;

    if(particle.m_state.m_type == kBullet)
    {
        particle.m_scale.x =   particle.m_state.m_lengthMultiplier
                             * min(min(1.0f, 0.1f * speed + 0.1f), alpha);
    }
    else
    {
        particle.m_scale.x =   particle.m_state.m_lengthMultiplier
                             * min(min(1.0f, 0.2f * speed + 0.1f), alpha);
    }

    particle.m_orientation = Extensions::toAngle(vel);

    Vector2f pos = particle.m_position;
    int width = (int) constants::WINDOW_WIDTH;
    int height = (int) constants::WINDOW_HEIGHT;

    if(pos.x < 0)
    {
        vel.x = (float) fabs(vel.x);
    }
    else if(pos.x > width)
    {
        vel.x = (float) -fabs(vel.x);
    }

    if(pos.y < 0)
    {
        vel.y = (float) fabs(vel.y);
    }
    else if(pos.x > width)
    {
        vel.y = (float) -fabs(vel.y);
    }

    if(particle.m_state.m_type != kIgnoreGravity)
    {
		for(std::list<BlackHole*>::iterator j = EntityManager::getInstance()->m_blackHoles.begin();
            j != EntityManager::getInstance()->m_blackHoles.end();
            j++)
        {
            Vector2f pos = (*j)->getPosition() - pos;
            float distance = pos.length();
            Vector2f n = pos / distance;

            vel += 10000.0f * n / (distance * distance + 10000.0f);

            if(distance < 400)
            {
                vel += 45.0f * Vector2f(n.y, -n.x) / (distance + 100.0f);
            }
        }
    }

    if(fabs(vel.x) + fabs(vel.y) < 0.00000000001f)
    {
        vel = Vector2f(0, 0);
    }
    else if(particle.m_state.m_type == kEnemy)
    {
        vel *= 0.94f;
    }
    else
    {
        vel *= 0.96f + (float) fmod(fabs(pos.x), 0.94f);
    }

    particle.m_state.m_velocity = vel;
}
