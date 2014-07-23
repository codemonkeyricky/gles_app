
#include "IL/il.h"

class Entity
{
public:
    Entity();
    void draw();
    void setX();
    void setY();

private:
    void    textureLoad();
    int     x;
    int     y;
    GLuint  m_textureId;
};
