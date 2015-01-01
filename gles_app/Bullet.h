#pragma once

#include "Entity.h"

class Bullet
    : public Entity
{
public:
    Bullet(const Vector2f &position, const Vector2f &velocity);
    void update();
};
