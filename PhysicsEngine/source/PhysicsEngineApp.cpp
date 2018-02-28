#include "PhysicsEngineApp.h"
#include "Camera.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Physics\Sceen.h"
#include "Physics\Object.h"
#include "Physics\Sphere.h"
#include "Physics\Plain.h"
using namespace Physics;

#include <imgui.h>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

PhysicsEngineApp::PhysicsEngineApp() {

}

PhysicsEngineApp::~PhysicsEngineApp() {
	delete m_camera;
	delete m_sceen;
}

bool PhysicsEngineApp::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	m_camera = new Camera();
	m_camera->SetProjection(glm::radians(45.0f), (float)getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);
	m_camera->SetPosition(glm::vec3(10, 10, 10));
	m_camera->Lookat(glm::vec3(0, 0, 0));

	// set the sceen up
	m_sceen = new Sceen();
	//m_sceen->setGravity(vec3(0.0f, 9.8f, 0.0f));

	// make first heavy object
	m_sphere = new Sphere(vec3(10, 10.0f,0.0f), 2, 2, vec4(1.0f, 0.0f, 0.0f, 1.0f), false);
	m_sphere->SetVelocity(vec3(-3.0f, 0.0f, 0.0f));

	m_sceen->addObject(m_sphere);

	// make second light object
	Sphere *object2 = new Sphere(vec3(10.0f, 2.5f, 0.0f), 2, 1.0f,
								vec4(0.0f, 1.0f, 0.0f, 1.0f), false);
	m_sceen->addObject(object2);

	Plain * plain = new Plain(vec3(0, 0, 0), vec3(0, 1, 0), 100, vec4(0, 0, 1, 1), true);
	m_sceen->addObject(plain);

	//m_sceen->setGlobalForce(vec3(1.0f, 0.0f, 0.0f));

	return true;
}

void PhysicsEngineApp::shutdown() {

	Gizmos::destroy();
}

void PhysicsEngineApp::update(float deltaTime) {

	m_camera->Update(deltaTime);

	ImGui::Begin("gravity deBug");
	ImGui::Text("object Velocity x: %.2f", m_sphere->GetVelocity().x);
	ImGui::Text("object Velocity y: %.2f", m_sphere->GetVelocity().y);
	ImGui::Text("object Velocity z: %.2f", m_sphere->GetVelocity().z);
	ImGui::End();

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
						vec3(-10 + i, 0, -10),
						i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
						vec3(-10, 0, -10 + i),
						i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	m_sceen->applyGlobalForce();

	m_sceen->update(deltaTime);
}

void PhysicsEngineApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// arsk the sceen to add all its gizmos
	m_sceen->draw();

	// update perspective based on screen size
	Gizmos::draw(m_camera->GetProjectionView());
}