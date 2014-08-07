#pragma once

#include "Singleton.h"

class EnemySpawner
    : public Singleton<EnemySpawner>
{
protected:
    float m_inverseSpawnChance;
    float m_inverseBlackHoleChance;

protected:
    Vector2f GetSpawnPosition();

protected:
    EnemySpawner();

public:
    void update();
    void reset();

    friend class Singleton<EnemySpawner>;
};
