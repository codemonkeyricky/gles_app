#include "EntityManager.h"
#include "Entity.h"
#include "Bullet.h"
#include "PlayerShip.h"
#include "EnemySpawner.h"


EntityManager::EntityManager()
    : m_isUpdating(false)
{

}


int EntityManager::getCount() const
{
    return (int) m_entities.size();
}


void EntityManager::add(
    Entity * entity
    )
{
    if(!m_isUpdating)
    {
        addEntity(entity);
    }
    else
    {
        m_addedEntities.push_back(entity);
    }
}


void EntityManager::addEntity(
    Entity * entity
    )
{
    m_entities.push_back(entity);

    switch(entity->getKind())
    {
    case Entity::kBullet: m_bullets.push_back((Bullet *) entity); break; 
	case Entity::kEnemy: m_enemies.push_back((Enemy *) entity); break; 
    default: break;
    }
}


void EntityManager::update()
{
    m_isUpdating = true;

	handleCollisions(); 

    // Update existing entities.
    for(std::list<Entity *>::iterator iter = m_entities.begin();
        iter != m_entities.end();
        iter++)
    {
        (*iter)->update();
        if((*iter)->isExpired())
        {
            *iter = NULL;
        }
    }

    m_isUpdating = false;

    // Add new entities.
    for(std::list<Entity *>::iterator iter = m_addedEntities.begin();
        iter != m_addedEntities.end();
        iter++)
    {
        addEntity(*iter);
    }

    // Clear new entities list.
    m_addedEntities.clear();

    // Remove all NULL elements.
    m_entities.remove(NULL);

    // Walk the bullet list.
    for(std::list<Bullet *>::iterator iter = m_bullets.begin();
        iter != m_bullets.end();
        iter++)
    {
        if((*iter)->isExpired())
        {
            delete *iter;
            *iter = NULL;
        }
    }
    m_bullets.remove(NULL);

	// Walk the enemies list.
    for(std::list<Enemy *>::iterator iter = m_enemies.begin();
        iter != m_enemies.end();
        iter++)
    {
        if((*iter)->isExpired())
        {
            delete *iter;
            *iter = NULL;
        }
    }
    m_enemies.remove(NULL);
}


bool EntityManager::isColliding(Entity *a, Entity *b)
{
    float radius = a->getRadius() + b->getRadius();

    // TODO:
    // Make sure both a and b are not expired... and..??
    return !a->isExpired() && !b->isExpired() && a->getPosition().distanceSquared(b->getPosition()) < radius * radius;
}


void EntityManager::draw()
{
    for(std::list<Entity *>::iterator iter = m_entities.begin();
        iter != m_entities.end();
        iter++)
    {
        (*iter)->draw();
    }
}


void EntityManager::handleCollisions()
{
    // Evade from other enemies.
    for(std::list<Enemy*>::iterator i = m_enemies.begin(); i != m_enemies.end(); i++)
    {
        for(std::list<Enemy*>::iterator j = m_enemies.begin(); j != m_enemies.end(); j++)
        {
            if(isColliding(*i, *j))
            {
                (*i)->handleCollision(*j);
                (*j)->handleCollision(*i);
            }
        }
    }

    // Determine shot enemies.
    for(std::list<Enemy*>::iterator i = m_enemies.begin(); i != m_enemies.end(); i++)
    {
        for(std::list<Bullet*>::iterator j = m_bullets.begin(); j != m_bullets.end(); j++)
        {
            if(isColliding(*i, *j))
            {
                (*i)->wasShot();
                (*i)->setExpired();
            }
        }
    }

    // Determine enemy crashing into player.
    for(std::list<Enemy*>::iterator i = m_enemies.begin(); i != m_enemies.end(); i++)
    {
        if(isColliding(*i, PlayerShip::getInstance()) && (*i)->getIsActive())
        {
            PlayerShip::getInstance()->kill();

            for(std::list<Enemy*>::iterator j = m_enemies.begin(); j != m_enemies.end(); j++)
            {
                (*j)->wasShot();
            }

            EnemySpawner::getInstance()->reset();
            break;
        }
    }
}
