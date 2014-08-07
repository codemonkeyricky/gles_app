#include "constants.hpp"
#include "Vector.h"
#include "Math.h"
#include "EnemySpawner.h"
#include "EntityManager.h"
#include "Enemy.h"
#include "PlayerShip.h"


Vector2f EnemySpawner::GetSpawnPosition()
{
    Vector2f pos;


    do
    {
        pos = Vector2f(Math::random() * constants::WINDOW_WIDTH,
                       Math::random() * constants::WINDOW_HEIGHT);

    } while (pos.distanceSquared(PlayerShip::getInstance()->getPosition()) < 250 * 250);

    return pos;
}


EnemySpawner::EnemySpawner()
    : m_inverseSpawnChance(90),
      m_inverseBlackHoleChance(600)
{

}


void EnemySpawner::update()
{
    if(!PlayerShip::getInstance()->getIsDead() && EntityManager::getInstance()->getCount() < 200)
    {
        if(int(Math::random() * m_inverseSpawnChance) == 0)
        {
            EntityManager::getInstance()->add(Enemy::createSeeker(GetSpawnPosition()));
        }

        if(int(Math::random() * m_inverseSpawnChance) == 0)
        {
            EntityManager::getInstance()->add(Enemy::createWanderer(GetSpawnPosition()));
        }

        if(   (EntityManager::getInstance()->getBlackHoleCount() < 2)
           && (int(Math::random() * m_inverseBlackHoleChance) == 0))
        {
            EntityManager::getInstance()->add(new BlackHole(GetSpawnPosition()));
        }
    }

    if(m_inverseSpawnChance > 30)
    {
        m_inverseSpawnChance -= 0.005f;
    }
}


void EnemySpawner::reset()
{
    m_inverseSpawnChance = 90;
}
