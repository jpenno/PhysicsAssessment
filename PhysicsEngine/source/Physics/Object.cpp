#include "Physics\Sphere.h"
#include "Physics\Plain.h"
#include "Physics\AABB.h"
#include <Gizmos.h>

using namespace Physics;

using glm::vec3;

#include <iostream>
using std::cout;
using std::endl;

Object::Object(ShapeType shape, vec3 pos, float mass, vec4 color, bool isStatic)
	: m_shape(shape), m_position(pos), m_mass(mass), m_color(color), m_isStatic(isStatic)
{
	m_velocity = vec3();
	m_acceleration = vec3();
	m_impulse = vec3();
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

		m_velocity += m_impulse;
		m_velocity += m_acceleration * deltaTime;
		m_position += m_velocity * deltaTime;

		// reset impulse to zero after adding it to the velocity
		m_impulse = vec3();

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
			return isCollidingAABBSphere((Aabb*) other, (Sphere*) this);
			break;

		case Physics::PLAIN:
			return isCollidingSpherePlain((Sphere*)this, (Plain*)other);
			break;
		}
		break;
	case Physics::PLAIN:
		switch (other->GetShape())
		{
		case Physics::SPHERE:
			return isCollidingSpherePlain((Sphere*)other, (Plain*)this);
			break;

		case Physics::AABB:
			return isCollidingAABBPlain((Aabb*)other, (Plain*)this);
			break;
		}
		break;
	case Physics::AABB:
		switch (other->GetShape())
		{
		case Physics::PLAIN:
			return isCollidingAABBPlain((Aabb*) this, (Plain*) other);
			break;
		case Physics::SPHERE:
			return isCollidingAABBSphere((Aabb*)this, (Sphere*)other);
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

bool Object::isCollidingSpherePlain(Sphere * sphere, Plain * plain)
{	
	// find distince to the plain 
	vec3 tmp = sphere->GetPosition() - plain->GetPosition();
	float distince = glm::dot(plain->getNormal(), tmp);

	if (distince < sphere->GetRadius()){
		//sphere->SetVelocity(vec3());
		return true;
	}
	return false;
}

bool Object::isCollidingAABBPlain(Aabb * aabb, Plain * plain)
{

	vec3 tmp = aabb->GetPosition() - plain->GetPosition();
	float distince = glm::dot(plain->getNormal(), tmp);
	
	if (distince < aabb->getSize().y) {
		return true;
	}
	if (distince < aabb->getSize().x) {
		return true;
	}
	if (distince < aabb->getSize().z) {
		return true;
	}
	return false;
}

bool Physics::Object::isCollidingAABBSphere(Aabb * aabb, Sphere * sphere)
{
	auto clamp = [](
		float & p,
		float maxX,
		float minx)
	{
		if (p > maxX)
			p = maxX;	
		if (p < minx)
			p = minx;
	};

	auto check = [&](
		vec3 spherPos,
		vec3 clampPos) -> bool
	{
		float distince = glm::distance(clampPos, sphere->GetPosition());

		if (distince < sphere->GetRadius())
			return true;

		return false;
	};

	vec3 tmp = sphere->GetPosition() - aabb->GetPosition();
	float distince1 = 0.0f;
	float distince2 = 0.0f;
	vec3 clampPos = vec3();
	clampPos = sphere->GetPosition();

	// do the top and bottom of the AABB *****************************************************
	// find distince to the to side of the AABB
	distince1 = (glm::dot(vec3(0,  1, 0), tmp) - aabb->getSize().y);
	distince2 = (glm::dot(vec3(0, -1, 0), tmp) - aabb->getSize().y);

	if ((distince1 < sphere->GetRadius()) &&
		(distince2 < sphere->GetRadius())) {
		// clam the x amd z to the max x and z in the AABB
		clamp(clampPos.x, aabb->getMax().x, aabb->getMin().x);
		clamp(clampPos.z, aabb->getMax().z, aabb->getMin().z);

		if (check(sphere->GetPosition(), clampPos))
		{
			float d1 = glm::distance(sphere->GetPosition().y, aabb->getMax().y);
			float d2 = glm::distance(sphere->GetPosition().y, aabb->getMin().y);
		
			if ( d1 < d2)
				clampPos.y = aabb->GetPosition().y + aabb->getSize().y;
	
			if ( d2 < d1)
				clampPos.y = aabb->GetPosition().y - aabb->getSize().y;
		
			aabb->setContactPoint(clampPos);
			return true;
		}
	}
	//****************************************************************************************

	// do the front and back of the AABB *****************************************************
	distince1 = (glm::dot(vec3( 1, 0, 0), tmp) - aabb->getSize().x);
	distince2 = (glm::dot(vec3(-1, 0, 0), tmp) - aabb->getSize().x);

	if ((distince1 < sphere->GetRadius()) &&
		(distince2 < sphere->GetRadius())) {
		// clamp the y amd z to the max y and z in the AABB
		clamp(clampPos.y, aabb->getMax().y, aabb->getMin().y);
		clamp(clampPos.z, aabb->getMax().z, aabb->getMin().z);

		if (check(sphere->GetPosition(), clampPos))
		{
			float d1 = glm::distance(sphere->GetPosition().x, aabb->getMax().x);
			float d2 = glm::distance(sphere->GetPosition().x, aabb->getMin().x);

			if (d1 < d2)
				clampPos.x = aabb->GetPosition().x + aabb->getSize().x;

			if (d2 < d1)
				clampPos.x = aabb->GetPosition().x - aabb->getSize().x;

			aabb->setContactPoint(clampPos);
			return true;
		}
	}
	//****************************************************************************************

	// do the Left and right of the AABB *****************************************************
	distince1 = (glm::dot(vec3(0, 0, 1), tmp) - aabb->getSize().x);
	distince2 = (glm::dot(vec3(0, 0, -1), tmp) - aabb->getSize().x);

	if ((distince1 < sphere->GetRadius()) &&
		(distince2 < sphere->GetRadius())) {
		// clamp the y amd z to the max y and z in the AABB
		clamp(clampPos.y, aabb->getMax().y, aabb->getMin().y);
		clamp(clampPos.x, aabb->getMax().x, aabb->getMin().x);

		if (check(sphere->GetPosition(), clampPos))
		{
			float d1 = glm::distance(sphere->GetPosition().z, aabb->getMax().z);
			float d2 = glm::distance(sphere->GetPosition().z, aabb->getMin().z);

			if (d1 < d2)
				clampPos.z = aabb->GetPosition().z + aabb->getSize().z;

			if (d2 < d1)
				clampPos.z = aabb->GetPosition().z - aabb->getSize().z;
			aabb->setContactPoint(clampPos);
			return true;
		}
	}
	//****************************************************************************************
	return false;
}
