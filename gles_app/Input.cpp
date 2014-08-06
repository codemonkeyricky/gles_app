#include <windows.h>
#include <conio.h>
#include "Input.h"
#include "Renderer.h"
#include "PlayerShip.h"

#define W_KEY	0x57
#define S_KEY	0x53
#define A_KEY	0x41
#define D_KEY	0x44

#define KEYDOWN(vk_code)    ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

void Input::GetMouseState()
{
    CURSORINFO  pci;
    BOOL        success;


    pci.cbSize = sizeof(pci);

    success = GetCursorInfo(&pci);
    assert(success);

    success = ScreenToClient(Renderer::getInstance()->m_hWindow, &pci.ptScreenPos);
    assert(success);

    m_mouseState = Vector2f(pci.ptScreenPos.x, pci.ptScreenPos.y);
}


void Input::GetKeyboardState()
{
   m_keyboardState[k_up]    = KEYDOWN(VK_UP)    ? true : false;
   m_keyboardState[k_down]  = KEYDOWN(VK_DOWN)  ? true : false;
   m_keyboardState[k_left]  = KEYDOWN(VK_LEFT)  ? true : false;
   m_keyboardState[k_right] = KEYDOWN(VK_RIGHT) ? true : false;

   m_keyboardState[k_w]     = KEYDOWN(W_KEY)    ? true : false;
   m_keyboardState[k_a]     = KEYDOWN(A_KEY)    ? true : false;
   m_keyboardState[k_s]     = KEYDOWN(S_KEY)    ? true : false;
   m_keyboardState[k_d]     = KEYDOWN(D_KEY)    ? true : false;
}


Input::Input()
    : m_mouseState(-1, -1),
      m_lastMouseState(-1, -1),
      m_isAimingWithMouse(false),
      m_leftEngaged(255),
      m_rightEngaged(255)
{
    unsigned int i;


    m_keyboardState.resize(8);
    m_lastKeyboardState.resize(8);
    m_freshKeyboardState.resize(8);

    for(i = 0; i < 8; i++)
    {
        m_keyboardState[i]      = false;
        m_lastKeyboardState[i]  = false;
        m_freshKeyboardState[i] = false;
    }
}


Vector2f Input::GetMouseAimDirection() const
{
    Vector2f dir = m_mouseState - PlayerShip::getInstance()->getPosition();

    if(dir == Vector2f(0, 0))
    {
        return Vector2f(0, 0);
    }

    return dir.normalize();
}


Vector2f Input::getMousePosition() const
{
    return m_mouseState;
}


void Input::update()
{
    m_lastKeyboardState = m_keyboardState;
    m_lastMouseState = m_mouseState;

    GetMouseState();
    GetKeyboardState();

    if(   m_keyboardState[k_left]
       || m_keyboardState[k_right]
       || m_keyboardState[k_up]
       || m_keyboardState[k_down])
    {
        m_isAimingWithMouse = false;
    }
    else if(m_mouseState != m_lastMouseState)
    {
        m_isAimingWithMouse = true;
    }
}


Vector2f Input::getMovementDirection() const
{
    Vector2f direction(0,0);

    if(m_keyboardState[k_a])
    {
        direction.x -= 1;
    }

    if(m_keyboardState[k_d])
    {
        direction.x += 1;
    }

    if(m_keyboardState[k_w])
    {
        direction.y -= 1;
    }

    if(m_keyboardState[k_s])
    {
        direction.y += 1;
    }

    if(direction.lengthSquared() > 1)
    {
        direction.normalize();
    }

    return direction;
}


Vector2f Input::getAimDirection() const
{
    if(!m_isAimingWithMouse)
    {
        Vector2f direction(0,0);

        if(m_keyboardState[k_left])
        {
            direction.x -= 1;
        }

        if(m_keyboardState[k_right])
        {
            direction.x += 1;
        }

        if(m_keyboardState[k_up])
        {
            direction.y -= 1;
        }

        if(m_keyboardState[k_down])
        {
            direction.y += 1;
        }

        if(direction == Vector2f(0,0))
        {
            return Vector2f(0,0);
        }
        else
        {
            return direction.normalize();
        }
    }

    return GetMouseAimDirection();
}
