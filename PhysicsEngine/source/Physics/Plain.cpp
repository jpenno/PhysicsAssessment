#include "..\..\include\Physics\Plain.h"
#include <Gizmos.h>

#include <iostream>

using namespace Physics;



Physics::Plain::Plain(vec3 position, vec3 normal, float mass, vec4 colour, bool isStatic) :
Object(Physics::ShapeType::PLAIN, position, mass, colour, isStatic),
m_normal(normal)
{
}

Plain::~Plain()
{
}

void Plain::draw()
{
	aie::Gizmos::addTri( vec3(  100, m_position.y, -100),
						 vec3( -100, m_position.y, -100),
						 vec3( -100, m_position.y,  100),
							m_color);

	aie::Gizmos::addTri(vec3( -100, m_position.y,  100),
						vec3(  100, m_position.y,  100),
						vec3(  100, m_position.y, -100),
						m_color);
}
