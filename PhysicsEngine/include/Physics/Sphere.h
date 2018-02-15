#pragma once
#include "Object.h"
#include <glm/glm.hpp>

using glm::vec3;
using glm::vec4;

namespace Physics
{
	class Sphere : public Object
	{
	public:
		Sphere(vec3 position, float radius, float mass, vec4 colour, bool isStatic);
		~Sphere();

		void draw();
		inline const float GetRadius() const { return m_radius; }
		inline void SetRadius(const float radius) { m_radius = radius; }

	protected:
		float m_radius;
	};
}
