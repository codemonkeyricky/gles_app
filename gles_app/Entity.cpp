#include <stdio.h>
#include <stdlib.h>

#include "GLES2/gl2.h"
#include "EGL/egl.h"

#include "assert.h"
#include "Entity.h"


Entity::Entity(
	void
	) :
	m_image(NULL),
	m_color(1,1,1,1),
	m_position(0, 0),
	m_velocity(0, 0),
	m_orientation(0),
	m_radius(0),
	m_isExpired(false),
	m_kind(kDontCare)
{
}


Entity::~Entity()
{

}


void Entity::draw(
    void
    )
{
	Renderer::getInstance()->draw(m_position, m_image->getSize(), m_orientation, m_image, m_color);
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
