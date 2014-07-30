#include <windows.h>

#include "Input.h"
#include "renderer.hpp"
#include "PlayerShip.h"

Input::Input()
{
}


Vector2f Input::getMouseAimDirection() const
{
    Vector2f dir = m_mouseState - PlayerShip::getInstance()->getPosition();

    if(dir == Vector2f(0, 0) || !m_leftClickState)
    {
        return Vector2f(0, 0);
    }

    return dir.normalize();
}

#define KEYDOWN(vk_code)    ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

void Input::update()
{
    CURSORINFO  pci;
    BOOL        success;


    pci.cbSize = sizeof(pci);

    success = GetCursorInfo(&pci);
    assert(success);

    success = ScreenToClient(Renderer::getInstance()->m_hWindow, &pci.ptScreenPos);
    assert(success);

    m_mouseState = Vector2f(pci.ptScreenPos.x, pci.ptScreenPos.y);
    m_leftClickState = KEYDOWN(VK_LBUTTON);
}


Vector2f Input::getMovementDirection() const
{
	return getMouseAimDirection();
}
