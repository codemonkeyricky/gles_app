#pragma once

#include <list>
#include "Entity.h"

class Enemy
    : public Entity
{
public:
    enum Behaviour
    {
        k_follow = 0,
        k_moveRandom
    };

protected:
    std::list<Behaviour> m_behaviors;
    float               m_randomDirection;
    int                 m_randomState;
    int                 m_pointValue;
    int                 m_timeUntilStart;

protected:
    void AddBehaviour(Behaviour b);
    void ApplyBehaviours();

public:
    Enemy(Texture *image, const Vector2f &position);

    void update();

    bool getIsActive();
    int getPointValue();

    static Enemy *createSeeker(const Vector2f &position);
    static Enemy *createWanderer(const Vector2f &position);

    void handleCollision(Enemy *other);
    void wasShot();

    bool followPlayer(float acceleration);
    bool moveRandomly();
};
