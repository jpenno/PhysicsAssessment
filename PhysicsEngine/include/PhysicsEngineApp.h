#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>

class Camera;
namespace Physics {
	class Sceen;
	class Sphere;
}


class PhysicsEngineApp : public aie::Application {
public:

	PhysicsEngineApp();
	virtual ~PhysicsEngineApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	// camera transforms
	Camera *m_camera = nullptr;

	Physics::Sceen *m_sceen = nullptr;
	Physics::Sphere *m_sphere = nullptr;
};