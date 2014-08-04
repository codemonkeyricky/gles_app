#include "constants.hpp"
#include "Enemy.h"
#include "Art.h"
#include "Math.h"
#include "PlayerStatus.h"
#include "PlayerShip.h"
#include "Rect.h"

void Enemy::AddBehaviour(Behaviour b)
{
    m_behaviors.push_back(b);
}


void Enemy::ApplyBehaviours()
{
    std::list<Behaviour>::iterator iter, iterNext;


    iter = m_behaviors.begin();
    iterNext = iter;

    while(iter != m_behaviors.end())
    {
        iterNext++;

        bool result = false;

        switch(*iter)
        {
        case k_follow:
            result = followPlayer(0.9f);
            break;
        case k_moveRandom:
            result = moveRandomly();
            break;
        }

        if(!result)
        {
            m_behaviors.erase(iter);
        }

        iter = iterNext;
    }
}

Enemy::Enemy(
    Texture        *image,
    const Vector2f &position
    )
{
    m_image     = image;
    m_position  = position;
    m_radius    = image->getSurfaceSize().width / 2.0f;
    m_color     = Color4f(0, 0, 0, 0);
    m_kind      = kEnemy;
}


void Enemy::update()
{
    if(m_timeUntilStart <= 0)
    {
        // Active enemy.
        ApplyBehaviours();
    }
    else
    {
        // Just spawned - transition into active.
        m_timeUntilStart--;

        m_color = Color4f(1, 1, 1, 1) * (1.0f - (float) m_timeUntilStart / 60.0);
    }

    m_position += m_velocity;
    m_position = Vector2f(Math::clamp(m_position.x,
                                      getSize().width / 2.0f,
                                      constants::WINDOW_WIDTH - getSize().width / 2.0f),
                          Math::clamp(m_position.y,
                                      getSize().height / 2.0f,
                                      constants::WINDOW_HEIGHT - getSize().height / 2.0f));

    m_velocity *= 0.8f;
}


bool Enemy::getIsActive()
{
    return m_timeUntilStart <= 0;
}


int Enemy::getPointValue()
{
    return m_pointValue;
}


Enemy * Enemy::createSeeker(const Vector2f &position)
{
    Enemy * enemy = new Enemy(Art::getInstance()->getSeeker(), position);

    enemy->AddBehaviour(k_follow);
    enemy->m_pointValue = 2;

    return enemy;
}


Enemy * Enemy::createWanderer(const Vector2f &position)
{
    Enemy * enemy = new Enemy(Art::getInstance()->getWanderer(), position);

    enemy->m_randomDirection = Math::random() * Math::PI * 2.0f;
    enemy->m_randomState = 0;
    enemy->AddBehaviour(k_moveRandom);

    return enemy;
}


void Enemy::handleCollision(Enemy * other)
{
    Vector2f d = m_position - other->m_position;

    m_velocity += 10.0f * d / (d.lengthSquared() + 1.0f);
}


void Enemy::wasShot()
{
    setExpired();

    PlayerStatus::getInstance()->addPoints(m_pointValue);
    PlayerStatus::getInstance()->increaseMultiplier();

    // TODO:
//    Sound * temp = Sound::getInstance()->getExplosion();
//    if(!temp->isPlaying())
//    {
//        temp->play(0, 1);
//    }
}


bool Enemy::followPlayer(float acceleration)
{
    if(!PlayerShip::getInstance()->getIsDead())
    {
        Vector2f temp = (PlayerShip::getInstance()->getPosition() - m_position);
        temp = temp * (acceleration / temp.length());

        m_velocity += temp;
    }

    if(m_velocity != Vector2f(0.0f, 0.0f))
    {
        m_orientation = atan2f(m_velocity.y, m_velocity.x);
    }

    return true;
}


bool Enemy::moveRandomly()
{
    if(m_randomState == 0)
    {
        m_randomDirection += Math::random() * 0.2f - 0.1f;
    }

    m_velocity += 0.4f * Vector2f(cosf(m_randomDirection), sinf(m_randomDirection));
    m_orientation -= 0.05f;

    Rectf bounds        =   Rectf(0, 0, constants::WINDOW_WIDTH, constants::WINDOW_WIDTH);
    bounds.location.x   -=  -m_image->getSurfaceSize().width / 2.0f - 1.0f;
    bounds.location.y   -=  -m_image->getSurfaceSize().height / 2.0f - 1.0f;
    bounds.size.width   +=  2.0f * (-m_image->getSurfaceSize().width / 2.0f - 1.0f);
    bounds.size.height  +=  2.0f * (-m_image->getSurfaceSize().height / 2.0f - 1.0f);

    // Make sure we stay in bound.
    if(!bounds.contains(Vector2f(m_position.x, m_position.y)))
    {
        Vector2f temp = Vector2f(constants::WINDOW_WIDTH, constants::WINDOW_HEIGHT) / 2.0f;
        temp -= m_position;
        m_randomDirection = atan2f(temp.y, temp.x) + Math::random() * Math::PI - Math::PI / 2.0f;
    }

    m_randomState = (m_randomState + 1) % 6;

    return true;
}
