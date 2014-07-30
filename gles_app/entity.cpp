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
	char           *pixel_data;
	unsigned int	width; 
	unsigned int	height; 


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

    loadTexture("test", &width, &height, &pixel_data);

	m_dimension.x = (float) width; 
	m_dimension.y = (float) height; 

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


Entity::Entity(
	void
	)
{
    textureLoad();
}


Entity::~Entity()
{

}


void Entity::draw(
    void
    )
{
    Renderer::getInstance()->draw(m_position, m_dimension, m_orientation, m_texture);
}


Vector2f Entity::getSize() const
{
    return m_dimension;
}


Vector2f Entity::getPosition() const
{
    return m_position;
}


Vector2f Entity::getVelocity() const
{
    return m_velocity;
}


void Entity::setVelocity(const Vector2f &nv)
{
    m_velocity = nv;
}


bool Entity::isExpired() const
{
    return m_isExpired;
}


Entity::Kind Entity::getKind() const
{
    return m_kind;
}


void Entity::setExpired()
{
    m_isExpired = true;
}


float Entity::getRadius() const
{
	return m_radius;
}
