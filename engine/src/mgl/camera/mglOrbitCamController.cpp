#include <mgl/camera/mglOrbitCamController.hpp>
#include <mgl/mglInputManager.hpp>
#include <utils/Logger.hpp>
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
		glm::vec3 translation = centerP - center->getPosition();
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
			float valX = -movement.xOffset * deltatime * mouseSensitivity;
			float valY = -movement.yOffset * deltatime * mouseSensitivity;

			if (InputManager::getInstance().isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
				float sense = 0.02;
				glm::vec3 translationXZ = camera->getRightV() * valX * sense;
				setCenter(center->getPosition() + 
					glm::vec3(translationXZ.x, -valY * sense, translationXZ.z));
			}
			else {
				rotateCamera(valX, YY);
				rotateCamera(valY, camera->getRightV());
			}


		}
	}
	 // zoom
	void OrbitCamController::registerDiscreteInputHandler() {
		InputManager::getInstance().registerMouseScrollCallBack(
			[this](GLfloat xOffset, GLfloat yOffset) {
				radius -= yOffset;
			    if (radius < 1.0f) radius = 1.0f;
				else {
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
		camera->lookAtFrom(center, newPos);
	}
}