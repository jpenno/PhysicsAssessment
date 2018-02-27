#include "..\..\include\Physics\Sceen.h"
#include "Physics\Object.h"
#include "Physics\Sphere.h"
#include "Physics\Plain.h"

#include <Gizmos.h>

#include <iostream>

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
		if (col.objA->GetIsStatic() || col.objB->GetIsStatic()){
			resolveStaticDynamicCollision(col);
		}
		else{
			resolveDynamicDynamicCollision(col);
		}

		// seperate the two objects
		// seperate two spheres
		if (col.objA->GetShape() == SPHERE && col.objB->GetShape() == SPHERE)
			seperateSphereSphere((Sphere*)col.objA, (Sphere*)col.objB);

		if (col.objA->GetShape() == PLAIN && col.objB->GetShape() == SPHERE)
			seperateSpherePlain((Sphere*)col.objB, (Plain*)col.objA);

		if (col.objA->GetShape() == SPHERE && col.objB->GetShape() == PLAIN)
			seperateSpherePlain((Sphere*)col.objA, (Plain*)col.objB);
	}
	m_ofCollision.clear();
}

void Physics::Sceen::resolveStaticDynamicCollision(Collision  col)
{
	vec3 n = col.objA->GetPosition() - col.objB->GetPosition();
	n = glm::normalize(n);

	vec3 nonstaticVelocity;
	vec3 plainNormal;
	if (col.objA->GetIsStatic())
	{
		nonstaticVelocity = col.objB->GetVelocity();
		Plain * plain = (Plain*)col.objA;
		plainNormal = plain->getNormal();
	}
	else
	{
		nonstaticVelocity = col.objA->GetVelocity();
		Plain * plain = (Plain*)col.objB;
		plainNormal = plain->getNormal();
	}

	vec3 velocity = nonstaticVelocity - (1 + 1) * glm::dot(nonstaticVelocity, plainNormal) * plainNormal;

	if (!col.objA->GetIsStatic())
		col.objA->SetVelocity(velocity);

	if (!col.objB->GetIsStatic())
		col.objB->SetVelocity(velocity);
}

void Physics::Sceen::resolveDynamicDynamicCollision(Collision  & col)
{
	// First, find the normalized vector n from the center of 
	// objA to the center of objB
	vec3 n = col.objA->GetPosition() - col.objB->GetPosition();
	n = glm::normalize(n);

	// Find the length of the component of each of the movement
	// vectors along n. 
	float reltiveVelocityA = glm::dot(col.objA->GetVelocity(), n);
	float reltiveVelocityB = glm::dot(col.objB->GetVelocity(), n);

	// calculate the force of the collision
	float optimizedP = (2.0f * (reltiveVelocityA - reltiveVelocityB)) /
		(col.objA->GetMass() + col.objB->GetMass());

	// calculate the new movement vector for object A
	vec3 va = col.objA->GetVelocity() - optimizedP * col.objB->GetMass() * n;

	// calculate the new movement vector for object B
	vec3 vb = col.objB->GetVelocity() + optimizedP * col.objA->GetMass() * n;

	if (!col.objA->GetIsStatic())
		col.objA->SetVelocity(va);

	if (!col.objB->GetIsStatic())
		col.objB->SetVelocity(vb);
}

void Sceen::seperateSphereSphere(Sphere * sa, Sphere * sb)
{
	float radi = sa->GetRadius() + sb->GetRadius();
	float distince = glm::distance(sb->GetPosition(), sa->GetPosition());

	if (distince < radi)
	{
		// find out how much they overlap
		float overlap = radi - distince;

		// check if ever sphere is static
		if (sa->GetIsStatic() || sb->GetIsStatic())
		{			
			// onley move the non static sphere
			if (!sa->GetIsStatic())
				sa->SetPosition(sa->GetPosition() - overlap);
		
			if (!sb->GetIsStatic())
				sb->SetPosition(sb->GetPosition() + overlap );
		}
		else
		{
			// move the objects apart
			sa->SetPosition(sa->GetPosition() - (overlap / 2));
			sb->SetPosition(sb->GetPosition() + (overlap / 2));
		}
	}
}

void Physics::Sceen::seperateSpherePlain(Sphere * sphere, Plain * plain)
{
	// get the spheres distince to the plain
	float distince = glm::dot(plain->getNormal(), sphere->GetPosition());

	// get how far past the plan the sphere is
	float overLap = sphere->GetRadius() - distince;

	// moving the sphere the direction of the normal the overlat amount
	vec3 tmp =  overLap * plain->getNormal();

	sphere->SetPosition(sphere->GetPosition() + tmp);
}
