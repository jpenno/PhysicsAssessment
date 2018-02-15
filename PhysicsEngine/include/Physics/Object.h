#pragma once
#include <glm/glm.hpp>
using glm::vec3;
using glm::vec4;

namespace Physics {
	enum ShapeType { SPHERE, PLAIN,AABB };
	class Sphere;

	class Object
	{
	protected:
		Object(ShapeType shape, vec3 pos, float mass, vec4 color, bool isStatic);

	public:
		virtual ~Object() = 0;


		void update(float deltaTime);
		void applyForce(const glm::vec3 & forece);

		virtual void draw() = 0;
		bool isColliding(Object *other);


		inline const vec3 & GetPosition() const { return m_position; }
		inline const vec3 & GetVelocity() const { return m_velocity; }
		inline const vec3 & GetAcceleration() const { return m_acceleration; }
		inline const ShapeType GetShape() const { return m_shape; }

		const float GetMass() { return m_mass; }
		const float GetFriction() { return m_friction; }

		void SetPosition(const vec3 a_pos) { m_position = a_pos; }
		void SetVelocity(const vec3 a_velocity) { m_velocity = a_velocity; }
		void SetAcceleration(const vec3 a_acceleration) { m_acceleration = a_acceleration; }
		void SetMass(const float a_mass){m_mass = a_mass;}
		void SetFriction(const float a_friction) { m_friction = a_friction; }

	protected:
		vec3 m_position;
		vec3 m_velocity;
		vec3 m_acceleration;

		ShapeType m_shape;
		vec4 m_color;

		bool m_isStatic;

		float m_mass = 1.0f;
		float m_friction = 0.3f;

		bool isCollidingSphereSphere(Sphere *objecta, Sphere * objectb);

	};
}