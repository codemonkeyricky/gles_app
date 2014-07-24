
#include <stdlib.h>

#include "renderer.hpp"
#include "entity.hpp"

//#include <il/il.h>

#if 0
const float aTexCoords[] =
{
    0.0, 0.0,
    1.0, 0.0,
    1.0, 1.0,
    0.0, 1.0,
};

const unsigned short aIndices[] =
{
    0, 2, 1,    // Triangle 1
    0, 3, 2     // Triangle 2
};

const float aVertices[] =
{
    0.0, 0.0,   // 0 
    1.0, 0.0,   // 1
    1.0, 1.0,   // 2
    0.0, 1.0,   // 3
};

const float aColours[] =
{
    1.0, 0.0, 0.0, /* red */
    0.0, 0.0, 1.0, /* blue */
    0.0, 1.0, 0.0, /* green */
    1.0, 1.0, 0.0, /* yellow */
};
#endif


int main(
    int     argc,
    char  **argv
    )
{
    Renderer   *renderer;
    Entity     *entity;
    bool        bDone = false;
    MSG         sMessage;


    renderer    = new Renderer();
    entity      = new Entity(renderer);

    /* Enter event loop */
    while(!bDone) 
    {
        if(PeekMessage(&sMessage, NULL, 0, 0, PM_REMOVE)) 
        {
            if(sMessage.message == WM_QUIT) 
            {
                bDone = 1;
            } 
            else 
            {
                TranslateMessage(&sMessage);
                DispatchMessage(&sMessage);
            }
        }

        entity->draw();

        renderer->render();

        Sleep(20);
    }

    return 0;
}
