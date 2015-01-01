#include "constants.hpp"
#include "PlayerShip.h"
#include "Bullet.h"
#include "EntityManager.h"
#include "Math.h"
#include "Matrix.h"
#include "Input.h"
#include "Art.h"
#include "PlayerStatus.h"
#include "Extensions.h"
#include "Timer.h"
#include "ParticleManager.h"


const int PlayerShip::kCooldownFrames = 6;

void PlayerShip::MakeExhaustFire()
{
    if(m_velocity.lengthSquared() > 0.1f)
    {
        m_orientation = Extensions::toAngle(m_velocity);

        float cosA = cosf(m_orientation);
        float sinA = sinf(m_orientation);

        Matrix2x2f rot(Vector2f(cosA, sinA),
                       Vector2f(-sinA, cosA));

        float t = Timer::getTimeMS() / 1000.0f;

        Vector2f baseVel = Extensions::scaleTo(m_velocity, -3);

        Vector2f perpVel = Vector2f(baseVel.y, -baseVel.x) * (0.6f * (float)sinf(t*10.0f));
        Color4f sideColor(0.78f, 0.15f, 0.04f, 1);
        Color4f midColor(1.0f, 0.73f, 0.12f, 1);
        Vector2f pos = m_position + rot * Vector2f(-25, 0);    // position of the ship's exhaust pipe.
        const float alpha = 0.7f;

        // middle particle stream
        Vector2f velMid = baseVel + Extensions::nextVector2(0, 1);
        ParticleManager::getInstance()->createParticle(Art::getInstance()->getLineParticle(),
                                                       pos,
                                                       Color4f(1,1,1,1) * alpha,
                                                       60.0f,
                                                       Vector2f(0.5f, 1),
                                                       ParticleState(velMid, ParticleState::kEnemy));
        ParticleManager::getInstance()->createParticle(Art::getInstance()->getGlow(),
                                                       pos,
                                                       midColor * alpha,
                                                       60.0f,
                                                       Vector2f(0.5f, 1),
                                                       ParticleState(velMid, ParticleState::kEnemy));

        // side particle streams
        Vector2f vel1 = baseVel + perpVel + Extensions::nextVector2(0, 0.3f);
        Vector2f vel2 = baseVel - perpVel + Extensions::nextVector2(0, 0.3f);
        ParticleManager::getInstance()->createParticle(Art::getInstance()->getLineParticle(),
                                                       pos,
                                                       Color4f(1,1,1,1) * alpha,
                                                       60.0f,
                                                       Vector2f(0.5f, 1),
                                                       ParticleState(vel1, ParticleState::kEnemy));
        ParticleManager::getInstance()->createParticle(Art::getInstance()->getLineParticle(),
                                                       pos,
                                                       Color4f(1,1,1,1) * alpha,
                                                       60.0f,
                                                       Vector2f(0.5f, 1),
                                                       ParticleState(vel2, ParticleState::kEnemy));

        ParticleManager::getInstance()->createParticle(Art::getInstance()->getGlow(),
                                                       pos,
                                                       sideColor * alpha,
                                                       60.0f,
                                                       Vector2f(0.5f, 1),
                                                       ParticleState(vel1, ParticleState::kEnemy));
        ParticleManager::getInstance()->createParticle(Art::getInstance()->getGlow(),
                                                       pos,
                                                       sideColor * alpha,
                                                       60.0f,
                                                       Vector2f(0.5f, 1),
                                                       ParticleState(vel2, ParticleState::kEnemy));
    }
}


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

        MakeExhaustFire();

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
