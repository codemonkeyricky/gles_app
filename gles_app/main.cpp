
#include <stdlib.h>

#include "renderer.hpp"
#include "entity.hpp"
#include "EntityManager.h"
#include "PlayerShip.h"


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
		// Update entities held by entity manager.
		EntityManager::getInstance()->update(); 

		// Draw all entities held by entity manager.
		EntityManager::getInstance()->draw(); 

		// Kick the renderer.
		Renderer::getInstance()->render();

        Sleep(20);
    }

    return 0;
}
