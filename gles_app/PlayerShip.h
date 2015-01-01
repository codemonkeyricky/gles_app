#pragma once

#include "Entity.h"
#include "Singleton.h"

class PlayerShip 
	: public Entity, 
      public Singleton<PlayerShip>
{

protected:
    static const int kCooldownFrames;

    int m_cooldownRemaining;
    int m_framesUntilRespawn;

protected:
    PlayerShip();
    void MakeExhaustFire();

public:
    void update();
    void draw();

    bool getIsDead();
    void kill();

	friend class Singleton<PlayerShip>; 
};
