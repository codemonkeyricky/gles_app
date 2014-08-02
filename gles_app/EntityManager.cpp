#include "EntityManager.h"
#include "Entity.h"
#include "Bullet.h"


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
    case Entity::kBullet:
        m_bullets.push_back((Bullet *) entity);
        break;

    default:
        break;
    }
}


void EntityManager::update()
{
    m_isUpdating = true;

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
