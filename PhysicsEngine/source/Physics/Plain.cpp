#include "..\..\include\Physics\Plain.h"
#include <Gizmos.h>

#include <iostream>

using namespace Physics;

Plain::Plain(vec3 position, vec3 normal, float mass, vec4 colour, bool isStatic) :
Object(Physics::ShapeType::PLAIN, position, mass, colour, isStatic),
m_normal(normal)
{
}

Plain::~Plain()
{
}

void Plain::draw()
{	
	if (m_normal.y == 1) // draw ground plain
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
	else if (m_normal.y == -1) // draw routh plain
	{		
		aie::Gizmos::addTri(vec3( -100, m_position.y,  100),
							vec3( -100, m_position.y, -100),
							vec3(  100, m_position.y, -100),
							m_color);
		
		aie::Gizmos::addTri(vec3(  100, m_position.y, -100),
							vec3(  100, m_position.y,  100),
							vec3( -100, m_position.y,  100),
							m_color);
	}
	else if (m_normal.x == 1) // draw wall plain
	{
		aie::Gizmos::addTri(vec3(m_position.x, -100, 100),
							vec3(m_position.x, -100, -100),
							vec3(m_position.x, 100, -100),
							m_color);

		aie::Gizmos::addTri(vec3(m_position.x, 100, -100),
							vec3(m_position.x, 100, 100),
							vec3(m_position.x, -100, 100),
							m_color);
	}
	else if (m_normal.x == -1) // draw wall plain
	{		
		aie::Gizmos::addTri(vec3(m_position.x, 100, -100),
							vec3(m_position.x, -100, -100),
							vec3(m_position.x, -100, 100),
							m_color);
		
		aie::Gizmos::addTri(vec3(m_position.x, -100, 100),
							vec3(m_position.x, 100, 100),
							vec3(m_position.x, 100, -100),
							m_color);
	}
	else if (m_normal.z == 1) // draw wall plain
	{
		aie::Gizmos::addTri(vec3( -100,  100, m_position.z ),
							vec3( -100, -100, m_position.z ),
							vec3(  100, -100, m_position.z ),
							m_color);

		aie::Gizmos::addTri(vec3( 100, -100, m_position.z ),
							vec3( 100,  100, m_position.z ),
							vec3(-100,  100, m_position.z ),
							m_color);
	}
	else if (m_normal.z == -1) // draw wall plain
	{		
		aie::Gizmos::addTri(vec3( 100, -100, m_position.z),
							vec3(-100, -100, m_position.z),
							vec3(-100,  100, m_position.z),
							m_color);
		
		aie::Gizmos::addTri(vec3(-100,  100, m_position.z),
							vec3( 100,  100, m_position.z),
							vec3( 100, -100, m_position.z),
							m_color);
	}
}
