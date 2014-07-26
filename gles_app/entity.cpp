#include <stdio.h>
#include <stdlib.h>

#include "assert.h"

#include "GLES2/gl2.h"
#include "EGL/egl.h"

#include "entity.hpp"

void loadTexture(
    char           *path,
    unsigned int   *width,
    unsigned int   *height,
    char          **pixels
    )
{
    ILuint  texid;
    char   *location = "C:/cygwin/home/ricky/dev/gles_app/gles_app/player.png";

    ilInit();

    ilGenImages(1, &texid);
    ilBindImage(texid);
    ilLoadImage((const wchar_t *) location);

    *width = ilGetInteger(IL_IMAGE_WIDTH);
    *height = ilGetInteger(IL_IMAGE_HEIGHT);

    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

    *pixels = (char *) malloc((*width) * (*height) * 4);

    memcpy(*pixels, (void *) ilGetData(), (*width) * (*height) * 4);
}


void Entity::textureLoad(
    void
    )
{
	char   *pixel_data;


    // Load texture.
    glGenTextures(1, &m_texture);
    assert(m_texture != NULL);

    // -------------------
    //  Setup texture
    // -------------------

    glBindTexture(GL_TEXTURE_2D, m_texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    loadTexture("test", &m_width, &m_height, &pixel_data);

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 (GLint) GL_RGBA,
                 (GLsizei) m_width,
                 (GLsizei) m_height,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 pixel_data);

    glBindTexture(GL_TEXTURE_2D, 0);
}


Entity::Entity(
    Renderer   *renderer
    ) : m_renderer(renderer), m_posX(0), m_posY(0), m_rot(0)
{
    textureLoad();
}


void Entity::draw(
    void
    ) const
{
    m_renderer->draw(m_posX, m_posY, m_width, m_height, m_rot, m_texture);
}
