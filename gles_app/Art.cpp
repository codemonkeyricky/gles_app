#include "Art.h"
#include "Surface.h"


Art::Art()
{
    m_player = new Texture(Surface("player.png"));
    m_seeker = new Texture(Surface("seeker.png"));
    m_wanderer = new Texture(Surface("wanderer.png"));
    m_bullet = new Texture(Surface("bullet.png"));
    m_pointer = new Texture(Surface("pointer.png"));

    // m_fontTexture = new Texture(Surface("font.png"));
}


Texture * Art::getPlayer() const
{
    return m_player;
}


Texture * Art::getSeeker() const
{
    return m_seeker;
}


Texture * Art::getWanderer() const
{
    return m_wanderer;
}


Texture * Art::getBullet() const
{
    return m_bullet;
}


Texture * Art::getPointer() const
{
    return m_bullet;
}


//Texture * Art::getFont() const
//{
//}
