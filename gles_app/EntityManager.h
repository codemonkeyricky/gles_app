#pragma once

#include <list>
#include "Singleton.h"
#include "Entity.h"
#include "Enemy.h"
#include "Bullet.h"


class EntityManager
    : public Singleton<EntityManager>
{
protected:
    std::list<Entity *> m_entities;
    std::list<Entity *> m_addedEntities;
    std::list<Bullet *> m_bullets;
    std::list<Enemy *>  m_enemies;
    bool m_isUpdating;

protected:
    EntityManager();

public:
    int getCount() const;

    void add(Entity *entity);
    void addEntity(Entity *entity);

    void update();
    void draw();

    void handleCollisions();
    bool isColliding(Entity *a, Entity *b);
//    bool isColliding(const Entity *a, const Entity *b);

	friend class Singleton<EntityManager>;
};
