#pragma once

#include <vector>
#include "Vector.h"
#include "Singleton.h"

class Input :
    public Singleton<Input>
{

protected:
    Vector2f            m_mouseState;
    Vector2f            m_lastMouseState;
    Vector2f            m_freshMouseState;
    std::vector<bool>   m_keyboardState;
    std::vector<bool>   m_lastKeyboardState;
    std::vector<bool>   m_freshKeyboardState;
    bool                m_isAimingWithMouse;
    bool                m_leftEngaged;
    bool                m_rightEngaged;

public:
    enum KeyType
    {
        k_up = 0,
        k_left,
        k_down,
        k_right,
        k_w,
        k_a,
        k_s,
        k_d
    };

protected:
    Vector2f GetMouseAimDirection() const;
    void GetMouseState();
    void GetKeyboardState();

protected:
    Input();

public:
    Vector2f    getMousePosition() const;
    void        update();

    bool        wasKeyPressed(KeyType) const;
    Vector2f    getMovementDirection() const;
    Vector2f    getAimDirection() const;

    friend class Singleton<Input>;
};
