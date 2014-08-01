#pragma once

#include "Vector.h"
#include "Singleton.h"

class Input :
    public Singleton<Input>
{

protected:
    Input();

    Vector2f    m_mouseState;
    bool        m_leftClickState;

public:
    void update();

    Vector2f getMovementDirection() const;
    Vector2f Input::getMouseAimDirection() const;

    friend class Singleton<Input>;
};
