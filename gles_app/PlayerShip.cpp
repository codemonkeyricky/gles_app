#include "constants.hpp"
#include "PlayerShip.h"
#include "Bullet.h"
#include "EntityManager.h"
#include "Math.h"
#include "Matrix.h"
#include "Input.h"
#include "Art.h"
#include "PlayerStatus.h"

const int PlayerShip::kCooldownFrames = 6;

PlayerShip::PlayerShip()
    : m_cooldownRemaining(0),
      m_framesUntilRespawn(0)
{
    m_image     = Art::getInstance()->getPlayer();
    m_position  = Vector2f(constants::WINDOW_WIDTH/2, constants::WINDOW_HEIGHT/2);
    m_radius    = 10;
}


void PlayerShip::update()
{
    if(getIsDead())
    {
        // Decrement frames.
        m_framesUntilRespawn--;
//        if(mFramesUntilRespawn == 0)
//        {
//            if (PlayerStatus::getInstance()->getLives() == 0)
//            {
//                PlayerStatus::getInstance()->reset();
//                mPosition = tVector2f(GameRoot::getInstance()->getViewportSize().width / 2.0f,
//                                      GameRoot::getInstance()->getViewportSize().height / 2.0f);
//            }
//        }
    }
    else
    {
        // Get aim direction.
		Vector2f aim = Input::getInstance()->getAimDirection();

        if(aim.lengthSquared() > 0 && m_cooldownRemaining <= 0)
        {
            m_cooldownRemaining = kCooldownFrames;
            float aimAngle = atan2f(aim.y, aim.x);

            float cosA = cosf(aimAngle);
            float sinA = sinf(aimAngle);
            Matrix2x2f aimMat(Vector2f(cosA, sinA),
                              Vector2f(-sinA, cosA));

            float randomSpread = Math::random() * 0.08f + Math::random() * 0.08f - 0.08f;
            Vector2f vel = 11.0f * (Vector2f(cosA, sinA) + Vector2f(randomSpread, randomSpread));

            Vector2f offset = aimMat * Vector2f(35, -8);
            EntityManager::getInstance()->add(new Bullet(m_position + offset, vel));

            offset = aimMat * Vector2f(35, 8);
            EntityManager::getInstance()->add(new Bullet(m_position + offset, vel));

            // TODO:
//            tSound* curShot = Sound::getInstance()->getShot();
//            if (!curShot->isPlaying())
//            {
//                curShot->play(0, 1);
//            }
        }

        if(m_cooldownRemaining > 0)
        {
            m_cooldownRemaining--;
        }

        const float kSpeed = 8;

		m_velocity += kSpeed * Input::getInstance()->getMovementDirection();
        m_position += m_velocity;
		m_position = Vector2f(Math::clamp(m_position.x, (float) getSize().x / 2.0f, (float) constants::WINDOW_WIDTH - getSize().x / 2),
                              Math::clamp(m_position.y, (float) getSize().y / 2.0f, (float) constants::WINDOW_HEIGHT - getSize().y / 2));

        if(m_position.lengthSquared() > 0)
        {
            m_orientation = atan2f(m_velocity.y, m_velocity.x);
        }

		m_velocity = Vector2f(0, 0); 
    }
}


void PlayerShip::draw()
{
    if(!getIsDead())
    {
        Entity::draw();
    }
}


bool PlayerShip::getIsDead()
{
    return m_framesUntilRespawn > 0;
}


void PlayerShip::kill()
{
//    PlayerStatus::getInstance()->removeLife();

    m_framesUntilRespawn = 60;
}
