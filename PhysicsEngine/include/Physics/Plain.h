#pragma once
#include "Object.h"


namespace Physics 
{
	class Plain : public Object
	{
	public:
		Plain(vec3 position, vec3 normal, float mass, vec4 colour, bool isStatic);

		~Plain();

		void draw();

		inline const vec3 getNormal() const { return m_normal; }

	protected:
		vec3 m_normal;
	};
}

