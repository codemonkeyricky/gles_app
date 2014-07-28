#pragma once

#include "entity.hpp"

class Bullet
    : public Entity
{
public:
    Bullet(const Vector2f &position, const Vector2f &velocity);
    void update();
};
