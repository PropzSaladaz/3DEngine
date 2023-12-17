#include <mgl/camera/mglOrbitCamController.hpp>
#include <mgl/mglInputManager.hpp>
#include <logger.hpp>
#include <glm/gtx/string_cast.hpp>

namespace mgl {
	OrbitCamController::OrbitCamController(Camera* camera) 
		: ICameraController(camera) {
		registerDiscreteInputHandler();
	}

	OrbitCamController::OrbitCamController(Camera* camera, const glm::vec3& centerP,
		GLfloat _radius) : OrbitCamController(camera) {
		setRadius(_radius);
		center = new Transform(centerP);
	}

	void OrbitCamController::setCenter(const glm::vec3& centerP) {
		glm::vec3 translation = center->getPosition() - centerP;
		center->setPosition(centerP);
		camera->translate(translation);
	}

	void OrbitCamController::setRadius(GLfloat radius) {
		GLfloat radiusDelta = this->radius - radius;
		camera->translate(camera->getFrontV() * radiusDelta);
		this->radius = radius;

	}

	void OrbitCamController::setMouseSensitivity(GLfloat sensitivity) {
		mouseSensitivity = sensitivity;
	}

	//////////////////////////////////////////////////////////////////////// Input
	// rotation on drag
	void OrbitCamController::handleContinuousInput(GLfloat deltatime) {
		if (InputManager::getInstance().isMouseBtnPressed(GLFW_MOUSE_BUTTON_LEFT)) {
			MouseMove movement = InputManager::getInstance().getMouseMovement();
			float val = -movement.xOffset * deltatime * mouseSensitivity;
			rotateCamera(val, YY);
		}
	}
	 // zoom
	void OrbitCamController::registerDiscreteInputHandler() {
		InputManager::getInstance().registerMouseScrollCallBack(
			[this](GLfloat xOffset, GLfloat yOffset) {
				radius -= yOffset;
			    if (radius < 1.0f) radius = 1.0f;
				else {
					util::Logger::Log("Scroll: " + std::to_string(xOffset));
					util::Logger::Log("Radius: " + std::to_string(radius));
					camera->zoom(yOffset);
				}
			});
	}
	//////////////////////////////////////////////////////////////////////// Private

	void OrbitCamController::rotateCamera(float degrees, const glm::vec3& axis) {
		glm::quat rot = glm::angleAxis(glm::radians(degrees), axis);
		rotation = glm::normalize(rot * rotation);
		glm::vec3 prevPos = camera->getPosition() - center->getPosition(); // move it back to origin
		glm::quat newPosQ = rot * glm::quat(0, prevPos) * glm::inverse(rot);
		glm::vec3 newPos = glm::vec3(newPosQ.x, newPosQ.y, newPosQ.z) + center->getPosition(); // move back to centerP

		util::Logger::Log("RotateCam - Prev. position: " + glm::to_string(prevPos) +
			" new wposition: " + glm::to_string(glm::vec3(newPos.x, newPos.y, newPos.z)));

		camera->lookAtFrom(center, newPos);
	}
}