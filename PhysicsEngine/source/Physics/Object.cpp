#include "Physics\Object.h"
#include "Physics\Sphere.h"
#include <Gizmos.h>
using namespace Physics;

// Testing 

using glm::vec3;

Object::Object(ShapeType shape, vec3 pos, float mass, vec4 color, bool isStatic)
	: m_shape(shape), m_position(pos), m_mass(mass), m_color(color), m_isStatic(isStatic)
{
	m_velocity = vec3();
	m_acceleration = vec3();
}

Object::~Object()
{
}

void Object::update(float deltaTime)
{
	if (!m_isStatic) // only run movememt if this is not a static object
	{
		// apply friction
		applyForce(-m_velocity * m_friction);

		m_velocity += m_acceleration * deltaTime;
		m_position += m_velocity * deltaTime;

		// reset acceleration to zero
		m_acceleration = vec3();
	}
}

void Object::applyForce(const glm::vec3 & forece)
{
	m_acceleration += forece / m_mass;
}

bool Object::isColliding(Object * other)
{
	// check if an object is overlaping another
	switch (m_shape) // whats my shape
	{
	case Physics::SPHERE:
		switch (other->GetShape())
		{
		case Physics::SPHERE:
			return isCollidingSphereSphere((Sphere*)this, (Sphere*)other);
			break;

		case Physics::AABB:
			//TODO: add collision deticion
			break;

		case Physics::PLAIN:
			//TODO: add collision deticion
			break;
		}
		break;
	}
	return false; // this is a catch for if we can't identify the objects
}

bool Object::isCollidingSphereSphere(Sphere * objecta, Sphere * objectb)
{
	assert(objecta != nullptr);
	assert(objectb != nullptr);

	// find the distince between centers
	float distince = glm::distance(objectb->GetPosition(), objecta->GetPosition());

	// add up the two radi
	float radi = objecta->GetRadius() + objectb->GetRadius();

	// is the distince smaller then the two radi
	return distince < radi;
}
