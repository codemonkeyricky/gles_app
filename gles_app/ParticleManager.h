#pragma once

#include <vector>
#include "Particle.h"
#include "Singleton.h"

class ParticleState;

class ParticleManager
    : public Singleton<ParticleManager>
{
protected:
    class CircularParticleArray
    {
    protected:
        std::vector<Particle>   m_list;
        size_t                  m_start;
        size_t                  m_count;    /// Track number of active particles.

    public:
        CircularParticleArray(int capacity)
        {
            m_list.resize((size_t) capacity);
        }
        size_t  getStart() {return m_start;}
        void    setStart(size_t value) { m_start = value % m_list.size(); }
        size_t  getCount() { return m_count; }
        void    setCount(size_t value) { m_count = value; }
        size_t  getCapacity() { return m_list.size(); }

        // Define [] operators.
        Particle &operator[](const size_t i)
        {
            return m_list[(m_start + i) % m_list.size()];
        }

        const Particle &operator[](const size_t i) const
        {
            return m_list[(m_start + i) % m_list.size()];
        }
    };

protected:
    CircularParticleArray   m_particleList;

protected:
    void Swap(
        CircularParticleArray &list,
        size_t index,
        size_t index2
        ) const;

public:
    ParticleManager();
    void update();
    void draw();

    void createParticle(
        Texture* texture,
        const Vector2f& position,
        const Color4f& tint,
        float duration,
        float scale,
        const ParticleState& state,
        float theta = 0
    );

    void createParticle(
        Texture* texture,
        const Vector2f& position,
        const Color4f& tint,
        float duration,
        Vector2f &scale,
        const ParticleState& state,
        float theta = 0
    );

    void clear();
    size_t getParticleCount();
};
