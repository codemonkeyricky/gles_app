#pragma once

#include <list>
#include "Singleton.h"
#include "Entity.h"
#include "Enemy.h"
#include "Bullet.h"
#include "BlackHole.h"


class EntityManager
    : public Singleton<EntityManager>
{
protected:
    std::list<Entity *>     m_entities;
    std::list<Entity *>     m_addedEntities;
    std::list<Bullet *>     m_bullets;
    std::list<Enemy *>      m_enemies;
    std::list<BlackHole *>  m_blackHoles;
    bool                    m_isUpdating;

protected:
    void KillPlayer();

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
    std::list<Entity*> EntityManager::getNearbyEntities(
        const Vector2f &pos,
        float           radius
    );

    unsigned int getBlackHoleCount() const;
    std::list<BlackHole*> getBlackHoles() const;

	friend class Singleton<EntityManager>;
	friend class ParticleState;
};
