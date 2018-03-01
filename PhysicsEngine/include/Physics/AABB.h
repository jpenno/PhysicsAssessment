#pragma once
#include "Physics\Object.h"
#include <iostream>

namespace Physics
{
	class Aabb : public Object
	{
	public:
		Aabb(vec3 position, vec3 size, float mass, vec4 colour, bool isStatic);
		~Aabb();

		void draw();
		void update(float deltatime);

		inline const vec3 getSize() const { return m_size; }
		inline const vec3 getMax() const { return m_max; }
		inline const vec3 getMin() const { return m_min; }
		inline const vec3 getContactPoint() const { return m_contactPoint; }

		inline void setContactPoint(const vec3 contactPoint) { m_contactPoint = contactPoint; }

	protected:
		vec3 m_size;
		vec3 m_max;
		vec3 m_min;
		vec3 m_contactPoint;
	};
}

