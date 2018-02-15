#include "..\..\include\Physics\Sphere.h"
#include <Gizmos.h>

using namespace Physics;


Sphere::Sphere(vec3 position, float radius, float mass, vec4 colour, bool isStatic) :
	m_radius(radius),
Object(Physics::ShapeType::SPHERE, position, mass, colour, isStatic)
{
}

Sphere::~Sphere()
{
}

void Sphere::draw()
{
	aie::Gizmos::addSphere(m_position, m_radius, 12, 12, m_color);
}
