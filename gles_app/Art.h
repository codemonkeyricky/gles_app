#pragma once

#include "Texture.h"
#include "Singleton.h"

class Art
    : public Singleton<Art>
{
protected:
    Texture *m_player;
    Texture *m_seeker;
    Texture *m_wanderer;
    Texture *m_bullet;
    Texture *m_pointer;
    Texture *m_blackHole;
    Texture *m_fontTexture;
    Texture *m_lineParticle;
    Texture *m_glow;
    Texture *m_pixel;

protected:
    Art();

public:
    Texture *getPlayer() const;
    Texture *getSeeker() const;
    Texture *getWanderer() const;
    Texture *getBullet() const;
    Texture *getPointer() const;
    Texture *getBlackHole() const;
    Texture *getLineParticle() const;
    Texture *getGlow() const;
    Texture *getPixel() const;

//    tSpriteFont getFont() const;

    friend class Singleton<Art>;
};
