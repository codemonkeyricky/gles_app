
#include <stdlib.h>

#include "Renderer.h"
#include "Entity.h"
#include "EntityManager.h"
#include "PlayerStatus.h"
#include "PlayerShip.h"
#include "Input.h"
#include "EnemySpawner.h"
#include "ParticleManager.h"

int main(
    int     argc,
    char  **argv
    )
{
    // Instantiate renderer. 
    Renderer::getInstance(); 

    // Instantiate entity manager, player ship, and add it to entity manager.
    EntityManager::getInstance()->add(PlayerShip::getInstance());

    while(1) 
    {
        // Update input.
        Input::getInstance()->update();

        // Update player status.
        PlayerStatus::getInstance()->update();

        // Update entities held by entity manager.
        EntityManager::getInstance()->update(); 

        // Update enemy spawner.
        EnemySpawner::getInstance()->update();

        // Update particle instance.
        ParticleManager::getInstance()->update();

        // Draw all entities held by entity manager.
        EntityManager::getInstance()->draw(); 

        // Draw all particles.
        ParticleManager::getInstance()->draw();

        // Kick the renderer.
        Renderer::getInstance()->render();

        Sleep(20);
    }

    return 0;
}
