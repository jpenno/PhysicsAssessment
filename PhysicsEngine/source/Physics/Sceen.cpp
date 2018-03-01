#include "..\..\include\Physics\Sceen.h"
#include "Physics\Sphere.h"
#include "Physics\Plain.h"
#include "Physics\AABB.h"

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
			//if (col.objA->GetShape() == SPHERE && col.objB->GetShape() == AABB)
			//	resolveSphereAABBCollision((Aabb*)col.objB, (Sphere*)col.objA);

			//if (col.objA->GetShape() == SPHERE && col.objB->GetShape() == AABB)
			//	resolveSphereAABBCollision((Aabb*)col.objB, (Sphere*)col.objA);

			resolveDynamicDynamicCollision(col);
		}

		// seperate the two objects
		// seperate two sphere **************************************************************
		if (col.objA->GetShape() == SPHERE && col.objB->GetShape() == SPHERE)
			seperateSphereSphere((Sphere*)col.objA, (Sphere*)col.objB);
		//***********************************************************************************

		// seperate Sphere Plain ************************************************************
		if (col.objA->GetShape() == PLAIN && col.objB->GetShape() == SPHERE)
			seperateSpherePlain((Sphere*)col.objB, (Plain*)col.objA);

		if (col.objA->GetShape() == SPHERE && col.objB->GetShape() == PLAIN)
			seperateSpherePlain((Sphere*)col.objA, (Plain*)col.objB);
		//***********************************************************************************

		// seperate AABB Plain **************************************************************
		if (col.objA->GetShape() == PLAIN && col.objB->GetShape() == AABB)
			seperateAABBPlain((Aabb*)col.objB, (Plain*)col.objA);

		if (col.objA->GetShape() == AABB && col.objB->GetShape() == PLAIN)
			seperateAABBPlain((Aabb*)col.objA, (Plain*)col.objB);
		//***********************************************************************************

		// seperate AABB Sphere ************************************************************
		if (col.objA->GetShape() == SPHERE && col.objB->GetShape() == AABB)
			seperateSphereAABB((Aabb*)col.objB, (Sphere*)col.objA);
	
		if (col.objA->GetShape() == SPHERE && col.objB->GetShape() == AABB)
			seperateSphereAABB((Aabb*)col.objB, (Sphere*)col.objA);
		//***********************************************************************************
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

	vec3 velocity = nonstaticVelocity - (1 + 0.7f) * glm::dot(nonstaticVelocity, plainNormal) * plainNormal;

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

void Sceen::resolveSphereAABBCollision(Aabb * aabb, Sphere * sphere)
{
	// First, find the normalized vector n from the center of 
	// objA to the center of objB
	vec3 n = sphere->GetPosition() - aabb->getContactPoint();
	n = glm::normalize(n);

	// Find the length of the component of each of the movement
	// vectors along n. 
	float reltiveVelocityA = glm::dot(sphere->GetVelocity(), n);
	float reltiveVelocityB = glm::dot(aabb->GetVelocity(), n);

	// calculate the force of the collision
	float optimizedP = (2.0f * (reltiveVelocityA - reltiveVelocityB)) /
		(sphere->GetMass() + aabb->GetMass());

	// calculate the new movement vector for object A
	vec3 va = sphere->GetVelocity() - optimizedP * aabb->GetMass() * n;

	// calculate the new movement vector for object B
	vec3 vb = aabb->GetVelocity() + optimizedP * sphere->GetMass() * n;

	if (!sphere->GetIsStatic())
		sphere->SetVelocity(va);

	if (!aabb->GetIsStatic())
		aabb->SetVelocity(vb);

}

void Sceen::seperateSphereSphere(Sphere * sa, Sphere * sb)
{
	float radi = sa->GetRadius() + sb->GetRadius();
	float distince = glm::distance(sb->GetPosition(), sa->GetPosition());

	if (distince < radi)
	{
		// find out how much they overlap
		float overlap = radi - distince;

		vec3 n = sb->GetPosition() - sa->GetPosition();
		n = glm::normalize(n);

		vec3 tmp = (overlap / 2) * n;


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
			sa->SetPosition(sa->GetPosition() - tmp);
			sb->SetPosition(sb->GetPosition() + tmp);
		}
	}
}

void Physics::Sceen::seperateSpherePlain(Sphere * sphere, Plain * plain)
{
	// get the spheres distince to the plain
	vec3 spereNewPos = sphere->GetPosition() - plain->GetPosition();
	float distince = glm::dot(plain->getNormal(), spereNewPos);

	// get how far past the plan the sphere is
	float overLap = sphere->GetRadius() - distince;

	// moving the sphere the direction of the normal the overlap amount
	vec3 tmp =  overLap * plain->getNormal();

	sphere->SetPosition(sphere->GetPosition() + tmp);
}

void Sceen::seperateAABBPlain(Aabb * aabb, Plain * plain)
{
	// get the spheres distince to the plain
	vec3 spereNewPos = aabb->GetPosition() - plain->GetPosition();
	float distince = glm::dot(plain->getNormal(), spereNewPos);

	// get the size of the side closest to the plain
	float  size = glm::dot(aabb->getSize() , plain->getNormal());
	if (size < 0){
		size *= -1;
	}

	// get how far past the plan the sphere is
	float overLap = size - distince;

	// moving the sphere the direction of the normal the overlap amount
	vec3 tmp = overLap * plain->getNormal();

	aabb->SetPosition(aabb->GetPosition() + tmp);
}

void Sceen::seperateSphereAABB(Aabb * aabb, Sphere * sphere)
{
	float distince = glm::distance(sphere->GetPosition(), aabb->getContactPoint());

	if (distince < sphere->GetRadius())
	{
		// find out how much they overlap
		float overlap = sphere->GetRadius() - distince;
		vec3 n = sphere->GetPosition() - aabb->getContactPoint() ;
		n = glm::normalize(n);

		vec3 tmp =( overlap / 2 )*  n;

		// check if ever sphere is static
		if (sphere->GetIsStatic() || aabb->GetIsStatic())
		{
			// onley move the non static sphere
			if (!sphere->GetIsStatic())
				sphere->SetPosition(sphere->GetPosition() - overlap);

			if (!aabb->GetIsStatic())
				aabb->SetPosition(aabb->GetPosition() + overlap);
		}
		else
		{
			// move the objects apart
			aabb->SetPosition(aabb->GetPosition() - tmp);
			sphere->SetPosition(sphere->GetPosition() + tmp);
		}
	}
}
