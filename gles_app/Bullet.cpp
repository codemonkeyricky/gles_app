#include "constants.hpp"
#include "Bullet.h"
#include "Rect.h"


Bullet::Bullet(
    const Vector2f &position,
    const Vector2f &velocity
    )
{
    loadTexture("C:/cygwin/home/ricky/dev/gles_app/gles_app/bullet.png");

    m_position = position;
    m_velocity = velocity;

    m_orientation = atan2f(m_velocity.y, m_velocity.x);
    m_radius = 8;
    m_kind = kBullet;
}


void Bullet::update(
    void
    )
{
    Rectf boundary;


    if(m_velocity.lengthSquared() > 0)
    {
        m_orientation = atan2f(m_velocity.y, m_velocity.x);
    }

    m_position += m_velocity;

    // Check if the bullet is still in boundary.
    boundary = Rectf(0, 0, (float) constants::WINDOW_WIDTH, (float) constants::WINDOW_HEIGHT);
    if(!boundary.contains(Vector2f(m_position.x, m_position.y)))
    {
        setExpired(); // m_isExpired = true;
    }
}
