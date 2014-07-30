#include "constants.hpp"
#include "PlayerShip.h"
#include "Bullet.h"
#include "EntityManager.h"
#include "Math.h"
#include "Matrix.h"
#include "Input.h"

const int PlayerShip::kCooldownFrames = 6;

PlayerShip::PlayerShip()
    : mCooldownRemaining(0),
      mFramesUntilRespawn(0)
{
    m_position   = Vector2f(constants::WINDOW_WIDTH/2, constants::WINDOW_HEIGHT/2);
    m_radius     = 10;
}


void PlayerShip::update()
{
    if(getIsDead())
    {
        // Decrement frames.
        mFramesUntilRespawn--;
    }
    else
    {
        // Get aim direction.
		Vector2f aim = Input::getInstance()->getMouseAimDirection();

        if(aim.lengthSquared() > 0 && mCooldownRemaining <= 0)
        {
            mCooldownRemaining = kCooldownFrames;
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

        if(mCooldownRemaining > 0)
        {
            mCooldownRemaining--;
        }

        const float kSpeed = 8;

		Vector2f temp = kSpeed * Input::getInstance()->getMovementDirection();
        m_velocity = m_velocity + temp;
        m_position = m_position + m_velocity;

        // TODO:
//        mPosition = tVector2f(tMath::clamp(mPosition.x, (float)getSize().x / 2.0f, (float)GameRoot::getInstance()->getViewportSize().x - getSize().x / 2),
//                              tMath::clamp(mPosition.y, (float)getSize().y / 2.0f, (float)GameRoot::getInstance()->getViewportSize().y - getSize().y / 2));

        if(m_position.lengthSquared() > 0)
        {
            m_orientation = atan2f(m_velocity.y, m_velocity.x);
        }
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
    return mFramesUntilRespawn > 0;
}


void PlayerShip::kill()
{
    mFramesUntilRespawn = 60;
}
