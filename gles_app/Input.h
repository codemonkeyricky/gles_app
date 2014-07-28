#pragma once

#include "Vector.h"
#include "Singleton.h"

class Input :
    public Singleton<Input>
{

protected:
    Input();

public:
    void update();

    Vector2f getMovementDirection() const;

    friend class Singleton<Input>;
};
