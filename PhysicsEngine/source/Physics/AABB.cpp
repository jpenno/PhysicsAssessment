#include "..\..\include\Physics\AABB.h"
#include <Gizmos.h>

using namespace Physics;



Aabb::Aabb(vec3 position, vec3 size, float mass, vec4 colour, bool isStatic) :
			Object(Physics::ShapeType::AABB, position, mass, colour, isStatic),
			m_size(size)
{
	m_max.x = m_position.x + (m_size.x );
	m_max.y = m_position.y + (m_size.y );
	m_max.z = m_position.z + (m_size.z );
							
	m_min.x = m_position.x - (m_size.x );
	m_min.y = m_position.y - (m_size.y );
	m_min.z = m_position.z - (m_size.z );
}

Aabb::~Aabb()
{
}

void Aabb::draw()
{
	aie::Gizmos::addAABBFilled(m_position, m_size, m_color);
}

void Aabb::update(float deltatime)
{
	Object::update(deltatime);
	m_max.x = m_position.x + (m_size.x);
	m_max.y = m_position.y + (m_size.y);
	m_max.z = m_position.z + (m_size.z);

	m_min.x = m_position.x - (m_size.x);
	m_min.y = m_position.y - (m_size.y);
	m_min.z = m_position.z - (m_size.z);
}
