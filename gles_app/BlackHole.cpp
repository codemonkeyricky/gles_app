#include <list>

#include "Art.h"
#include "BlackHole.h"
#include "EntityManager.h"
#include "Timer.h"
#include "Math.h"
#include "ColorUtil.h"
#include "MathUtil.h"
#include "Extensions.h"
#include "ParticleManager.h"
#include "Grid.h"

BlackHole::BlackHole(
    const Vector2f &position
    ) : m_sprayAngle(0)
{
    m_image     = Art::getInstance()->getBlackHole();
    m_position  = position;
    m_radius    = m_image->getSurfaceSize().width / 2.0f;
    m_kind      = kBlackHole;
}


void BlackHole::update()
{
    std::list<Entity*> entities = EntityManager::getInstance()->getNearbyEntities(m_position, 250);

    for(std::list<Entity*>::iterator iter = entities.begin();
        iter != entities.end();
        iter++)
    {
        if((*iter)->getKind() == kEnemy && !((Enemy*)(*iter))->getIsActive())
        {
            // Don't affect dead enemies.
        }
        else if((*iter)->getKind() == kBullet)
        {
            // Get directional vector from bullet to black hole.
            Vector2f temp = ((*iter)->getPosition() - m_position);

            // Repel bullets away from black hole.
            (*iter)->setVelocity((*iter)->getVelocity() + temp.normalize() * 0.3f);
        }
        else
        {
            Vector2f pos = m_position - (*iter)->getPosition();

            float length = pos.length();

            (*iter)->setVelocity((*iter)->getVelocity() + pos.normalize() * Math::mix(2.0f, 0.0f, length / 250.0f));
        }
    }

    m_sprayAngle -= Math::PI * 2.0f / 50.0f;

    Grid::getInstance()->applyImplosiveForce((float)sinf(m_sprayAngle / 2.0f) * 10 + 20, m_position, 200);
}

void BlackHole::draw()
{
    float scale = 1.0f + 0.1f * sinf(Timer::getTimeMS() * 10.0f / 1000.0f);

	Renderer::getInstance()->draw(m_position, m_image->getSize(), m_orientation, m_image, m_color, scale);
}


void BlackHole::wasShot()
{
    m_hitPoints--;

    if(m_hitPoints <= 0)
    {
        m_isExpired = true;
    }

    float hue = fmodf(3.0f / 1000.0f * Timer::getTimeMS(), 6);
    Color4f color = ColorUtil::HSVToColor(hue, 0.25f, 1);
    const int numParticles = 150;
    float startOffset = Extensions::nextFloat(0, Math::PI * 2.0f / numParticles);

    for(int i = 0; i < numParticles; i++)
    {
        Vector2f sprayVel = MathUtil::fromPolar(Math::PI * 2.0f * i / numParticles + startOffset, Extensions::nextFloat(8, 16));
        Vector2f pos = m_position + 2.0f * sprayVel;
        ParticleState state(sprayVel, ParticleState::kIgnoreGravity, 1.0f);

        ParticleManager::getInstance()->createParticle(Art::getInstance()->getLineParticle(), pos, color, 90, 1.5f, state);
    }

//    tSound* temp = Sound::getInstance()->getExplosion();
//    if (!temp->isPlaying())
//    {
//        temp->play(0, 1);
//    }
}


void BlackHole::kill()
{
    m_hitPoints = 0;

    wasShot();
}
