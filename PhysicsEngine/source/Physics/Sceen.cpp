#include "..\..\include\Physics\Sceen.h"
#include "Physics\Object.h"
#include <Gizmos.h>

using glm::vec4;
using namespace Physics;

Sceen::Sceen()
{
	// setting a default gravity 
	m_gravity = vec3(0.0f, -9.8f, 0.0f);

	m_globalForce = vec3();

	// delault for fied frame 100fps
	m_fixedTimeStep = 0.01f;
	m_accunulatedTime = 0.0f;
}

Sceen::~Sceen()
{
	for (auto object : m_objects)
	{
		delete object;
	}
}

void Sceen::update(float deltaTime)
{
	m_accunulatedTime += deltaTime;
	// using fixed time step for up date
	while (m_accunulatedTime >= m_fixedTimeStep)
	{
		applyGravity();

		for (auto object : m_objects)
		{
			object->update(m_fixedTimeStep);
		}

		checkCollisions();
		resolveCollision();

		m_accunulatedTime -= m_fixedTimeStep;
	}
}

void Sceen::addObject(Object * object)
{
	m_objects.push_back(object);
}

void Sceen::removeObject(Object * object)
{
	// find where the object is in the vector
	auto iter = std::find(m_objects.begin(), m_objects.end(), object);
	if (iter != m_objects.end())
	{
		m_objects.erase(iter);
	}
}

void Sceen::applyGlobalForce()
{
	for (auto object : m_objects)
	{
		object->applyForce(m_globalForce);
	}
}

void Sceen::draw()
{
	for (auto object : m_objects)
	{
		object->draw();
	}
}

void Sceen::applyGravity()
{
	for (auto object : m_objects)
	{
		// since gravity applies force based on mass
		// we multuply gravity by the object mass
		object->applyForce(m_gravity * object->GetMass());
	}
}

void Sceen::checkCollisions()
{
	// find all the collisions and place them in the collision vector

	for (auto object = m_objects.begin(); object != m_objects.end(); object++)
	{
		for (auto object2 = object + 1; object2 != m_objects.end(); object2++)
		{
			if ((*object)->isColliding(*object2))
			{
				Collision tempCollision;
				tempCollision.objA = *object;
				tempCollision.objB = *object2;
				m_ofCollision.push_back(tempCollision);
			}
		}
	}
}

void Sceen::resolveCollision()
{
	for (auto col : m_ofCollision)
	{
		col.objA->applyForce(col.objB->GetMass() * col.objB->GetVelocity());
		col.objB->applyForce(col.objA->GetMass() * col.objA->GetVelocity());
	}
	m_ofCollision.clear();


	// create a vector from A to B
	// creat a normilze the vector


	// caluacate the reltive velocity

	// find out how much of the relactive velocity goes along the collision vector

	// Start putting togather the impulse force
	// Elaasticty this will be an object variable, might need to average it from both objects
	// get the formula from our resources and clacuate j

	// create a functuion for applyImpulse(vec3) in our object
	// this applies force without muitplying by delta time

	// apply the j along the collision vector firection to object B
	// apply the j against the collision vector direction to object A


	// Seperate the two objects using what ever level of detail of seperation you want
}
