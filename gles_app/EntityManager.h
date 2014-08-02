#pragma once

#include <list>
#include "Singleton.h"
#include "Entity.h"

class Bullet;
class Enemy;

class EntityManager
    : public Singleton<EntityManager>
{
protected:
    std::list<Entity *>    m_entities;
    std::list<Entity *>    m_addedEntities;
    std::list<Bullet *>    m_bullets;
    bool m_isUpdating;

protected:
    EntityManager();

public:
    int getCount() const;

    void add(Entity *entity);
    void addEntity(Entity *entity);

    void update();
    void draw();

    bool isColliding(Entity *a, Entity *b);

	friend class Singleton<EntityManager>;
};
