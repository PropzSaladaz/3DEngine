#include <mgl/camera/mglOrbitCamController.hpp>
#include <mgl/mglInputManager.hpp>
#include <utils/Logger.hpp>

namespace mgl {
	OrbitCamController::OrbitCamController(Camera* camera) 
		: ICameraController(camera) {
		center = std::make_unique<Transform>(ORIGIN);
		registerDiscreteInputHandler();
	}

	OrbitCamController::OrbitCamController(Camera* camera, const math::vec3& centerP,
		f32 _radius) : OrbitCamController(camera) {
		setRadius(_radius);
		center = std::make_unique<Transform>(centerP);
	}

	void OrbitCamController::setCenter(const math::vec3& centerP) {
		if (!center) {
			center = std::make_unique<Transform>(centerP);
			camera->lookAtFrom(center.get(), camera->getPosition());
		} else {
			math::vec3 translation = centerP - center->getPosition();
			center->setPosition(centerP);
			camera->translate(translation);
		}
	}

	void OrbitCamController::setRadius(f32 radius) {
		f32 radiusDelta = this->radius - radius;
		camera->translate(camera->getFrontV() * radiusDelta);
		this->radius = radius;

	}

	void OrbitCamController::setMouseSensitivity(f32 sensitivity) {
		mouseSensitivity = sensitivity;
	}

	//////////////////////////////////////////////////////////////////////// Input
	// rotation on drag
	void OrbitCamController::handleContinuousInput(f32 deltatime) {
		if (InputManager::getInstance().isMouseBtnPressed(GLFW_MOUSE_BUTTON_LEFT)) {
			MouseMove movement = InputManager::getInstance().getMouseMovement();
			float valX = -movement.xOffset * deltatime * mouseSensitivity;
			float valY = -movement.yOffset * deltatime * mouseSensitivity;

			if (InputManager::getInstance().isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
				float sense = 0.02;
				math::vec3 translationXZ = camera->getRightV() * valX * sense;
				setCenter(center->getPosition() + 
					math::vec3(translationXZ.x(), -valY * sense, translationXZ.z()));
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
				[this](f32 xOffset, f32 yOffset) {
					radius -= yOffset;
				    if (radius < 1.0f) radius = 1.0f;
					else {
						camera->zoom(yOffset);
					}
				});
	}
	//////////////////////////////////////////////////////////////////////// Private

	void OrbitCamController::rotateCamera(f32 degrees, const math::vec3& axis) {
		math::quat rot = math::quat::fromAxisAngle(axis, math::radians(degrees));
		rotation = math::normalize(rot * rotation);
		if (!center) return;
		math::vec3 prevPos = camera->getPosition() - center->getPosition(); // move it back to origin
		math::quat newPosQ = rot * math::quat(prevPos, 0) * rot.inverse(); // TODO should it be inverse or conjugate?
		math::vec3 newPos = math::vec3(newPosQ.x(), newPosQ.y(), newPosQ.z()) + center->getPosition(); // move back to centerP
		camera->lookAtFrom(center.get(), newPos);
	}
}
