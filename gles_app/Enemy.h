#include "entity.hpp"

class Enemy
    : public Entity
{
public:
    enum Behavior
    {
        k_follow = 0,
        k_moveRandom
    };

protected:
    std::list<Behavior> m_behaviors;
    float               m_randomDirection;
    int                 m_randomState;
    int                 m_pointValue;
    int                 m_timeUntilStart;

protected:
    void AddBehaviour(Behavior b);
    void ApplyBehaviours();

public:
    Enemy(const Vector2f &position);

    void update();

    bool getIsActive();
    int getPointValue();

    static Enemy *createSeeker(const Vector2f &position);
    static Enemy *createWanderer(const Vector2f &position);

    void handleCollision(Enemey *other);
    void wasShot();

    bool followPlayer(float acceleration);
    bool moveRandomly();
};
