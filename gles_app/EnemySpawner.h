#pragma once

class EnemySpawner
    : public Singleton<EnemySpawner>
{
protected:
    float m_inverseSpawnChance;

protected:
    Vector2f GetSpawnPosition();

protected:
    EnemySpawner();

public:
//    void update();
    void reset();

    friend class Singleton<EnemySpawner>
};
