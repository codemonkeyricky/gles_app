#pragma once

#include "Entity.h"

class BlackHole
    : public Entity
{
protected:
    int m_hitPoints;

public:
    BlackHole(const Vector2f &position);

    void update();
    void draw();

    void wasShot();
    void kill();
};
