#include "ParticleManager.h"
#include "Renderer.h"


ParticleManager::ParticleManager(
    ) : m_particleList(1024 * 20)
{
}


void ParticleManager::Swap(
    ParticleManager::CircularParticleArray &list,
    size_t index1,
    size_t index2
    ) const
{
    Particle temp   = list[index1];
    list[index1]    = list[index2];
    list[index2]    = temp;
}


void ParticleManager::update(
    void
    )
{
    size_t  removalCount = 0;


    for(size_t i = 0; i < m_particleList.getCount(); i++)
    {
        Particle &ref = m_particleList[i];
        ref.m_state.updateParticle(ref);
        ref.m_percentLife -= 1.0f / ref.m_duration;

        Swap(m_particleList, i - removalCount, i);
        if(ref.m_percentLife < 0)
        {
            removalCount++;
        }
    }

    m_particleList.setCount(m_particleList.getCount() - removalCount);
}


void ParticleManager::draw()
{
    for(size_t i = 0; i < m_particleList.getCount(); i++)
    {
		Particle particle = m_particleList[(size_t) i]; 
			
			// particle.m_texture->getSurfaceSize() / 2;

        Renderer::getInstance()->draw(particle.m_position,
                                      particle.m_texture->getSurfaceSize(),
                                      particle.m_orientation,
									  particle.m_texture, 
                                      particle.m_color,
                                      particle.m_scale);
    }
}


void ParticleManager::createParticle(
    Texture* texture,
    const Vector2f& position,
    const Color4f& tint,
    float duration,
    float scale,
    const ParticleState& state,
    float theta
    )
{
    createParticle(texture, position, tint, duration, Vector2f(scale), state, theta);
}


void ParticleManager::createParticle(
    Texture* texture,
    const Vector2f& position,
    const Color4f& tint,
    float duration,
    Vector2f &scale,
    const ParticleState& state,
    float theta
    )
{
    size_t  index;


    if(m_particleList.getCount() == m_particleList.getCapacity())
    {
        // If at capacity, replace the oldest one.
        index = 0;
        m_particleList.setStart(m_particleList.getStart() + 1);
    }
    else
    {
        // If not at capacity, add at the end.
        index = m_particleList.getCount();
        m_particleList.setCount(m_particleList.getCount() + 1);
    }

    Particle &ref       = m_particleList[index];

    ref.m_texture       = texture;
    ref.m_position      = position;
    ref.m_color         = tint;

    ref.m_duration      = duration;
    ref.m_percentLife   = 1.0f;
    ref.m_scale         = scale;
    ref.m_orientation   = theta;
    ref.m_state         = state;
}


void ParticleManager::clear()
{
    m_particleList.setCount(0);
}

size_t ParticleManager::getParticleCount()
{
    return m_particleList.getCount();
}
