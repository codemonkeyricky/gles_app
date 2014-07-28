#pragma once

#include "entity.hpp"
#include "Singleton.h"

class PlayerShip 
	: public Entity, 
      public Singleton<PlayerShip>
{

protected:
    static const int kCooldownFrames;

    int mCooldownRemaining;
    int mFramesUntilRespawn;

protected:
    PlayerShip();

public:
    void update();
    void draw();

    bool getIsDead();
    void kill();

	friend class Singleton<PlayerShip>; 
};
