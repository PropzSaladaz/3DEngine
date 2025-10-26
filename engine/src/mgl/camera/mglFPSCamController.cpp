#include <mgl/camera/mglFPSCamController.hpp>
#include <mgl/mglInputManager.hpp>

namespace mgl {
	FPSCamController::FPSCamController(Camera* camera)
		: ICameraController(camera) {
		registerDiscreteInputHandler();
	}

	FPSCamController::FPSCamController(Camera* camera, GLfloat speed,
		GLfloat sensitivity) : FPSCamController(camera) {
		setMovementSpeed(speed);
		setMouseSensitivity(sensitivity);
	}

	void FPSCamController::setMovementSpeed(GLfloat speed) {
		movementSpeed = speed;
	}

	void FPSCamController::setMouseSensitivity(GLfloat sensitivity) {
		mouseSensitivity = sensitivity;
	}

////////////////////////////////////////////////////////////// input

	void FPSCamController::handleContinuousInput(GLfloat deltatime) {
		GLfloat moveAmount = deltatime * movementSpeed;
		glm::vec3 movementDir = { 0, 0, 0 };
		if (mgl::InputManager::getInstance().isKeyPressed(GLFW_KEY_W)) {
			movementDir += moveAmount * camera->getFrontV();
		}
		if (mgl::InputManager::getInstance().isKeyPressed(GLFW_KEY_A)) {
			movementDir += -moveAmount * camera->getRightV();
		}
		if (mgl::InputManager::getInstance().isKeyPressed(GLFW_KEY_S)) {
			movementDir += -moveAmount * camera->getFrontV();
		}
		if (mgl::InputManager::getInstance().isKeyPressed(GLFW_KEY_D)) {
			movementDir += moveAmount * camera->getRightV();
		}
		camera->translate(movementDir);
	}

	void FPSCamController::registerDiscreteInputHandler() {
		InputManager::getInstance().registerMouseOffsetCallBack(
			[this](GLfloat xOffset, GLfloat yOffset) {
				handleMouseOffset(xOffset, yOffset);
			});
	}

	void FPSCamController::handleMouseOffset(GLfloat xOffset, 
											GLfloat yOffset) {
		camera->yaw(-xOffset * mouseSensitivity);
		camera->pitch(-yOffset * mouseSensitivity);
	}
}