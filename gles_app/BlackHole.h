#pragma once

#include "Entity.h"

class BlackHole
    : public Entity
{
protected:
    int m_hitPoints;
    float   m_sprayAngle;

public:
    BlackHole(const Vector2f &position);

    void update();
    void draw();

    void wasShot();
    void kill();
};
