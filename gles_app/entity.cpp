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
//    ILenum  error;
    char *location = "C:/cygwin/home/ricky/dev/gles_app/gles_app/player.png";

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
    unsigned int    width;
    unsigned int    height;
	char           *pixel_data;


    // Load texture.
    glGenTextures(1, &m_textureId);
    assert(m_textureId != NULL);

    // -------------------
    //  Setup texture
    // -------------------

    glBindTexture(GL_TEXTURE_2D, m_textureId);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    loadTexture("test", &width, &height, &pixel_data);

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 (GLint) GL_RGBA,
                 (GLsizei) width,
                 (GLsizei) height,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 pixel_data);

    glBindTexture(GL_TEXTURE_2D, 0);
}


Entity::Entity()
{
    textureLoad();
}


void Entity::draw(
    )
{

}
