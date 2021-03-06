#pragma once
#include <vector>
#include <glm/glm.hpp>

using glm::vec3;
using std::vector;

namespace Physics {
	class Object;
	class Sphere;
	class Plain;
	class Aabb;

	struct Collision
	{
		Object * objA;
		Object * objB;
	};

	class Sceen
	{
	public:
		Sceen();
		~Sceen();

		void update(float deltaTime);
		void draw();

		inline const vec3 & getGravity() const { return m_gravity; }
		inline const vec3 & getGlobalForce() const { return m_globalForce; }

		inline void setGravity(const vec3 & gravity) { m_gravity = gravity; }
		inline void setGlobalForce(const vec3 & globalForce) { m_globalForce = globalForce; }


		void addObject(Object* object);
		void removeObject(Object* object);
		void applyGlobalForce();

	protected:
		vector<Object*> m_objects;
		vector<Collision> m_ofCollision;

		vec3 m_gravity;
		vec3 m_globalForce;

		float m_fixedTimeStep;
		float m_accunulatedTime;

	private:
		void applyGravity();
		void checkCollisions();

		void resolveCollision();
		void resolveStaticDynamicCollision(Collision  col);
		void resolveDynamicDynamicCollision(Collision  & col);
		void resolveSphereAABBCollision(Aabb * aabb, Sphere * sphere);

		void seperateSphereSphere(Sphere * sa, Sphere * sb);
		void seperateSpherePlain(Sphere * sphere, Plain * plain);
		void seperateAABBPlain(Aabb * aabb, Plain * plain);
		void seperateSphereAABB(Aabb * aabb, Sphere * sphere);
	};
}