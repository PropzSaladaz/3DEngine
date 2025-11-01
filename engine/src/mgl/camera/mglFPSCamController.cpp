#include <mgl/camera/mglFPSCamController.hpp>
#include <mgl/mglInputManager.hpp>

namespace mgl {
	FPSCamController::FPSCamController(Camera* camera)
		: ICameraController(camera) {
		registerDiscreteInputHandler();
	}

	FPSCamController::FPSCamController(Camera* camera, f32 speed,
		f32 sensitivity) : FPSCamController(camera) {
		setMovementSpeed(speed);
		setMouseSensitivity(sensitivity);
	}

	void FPSCamController::setMovementSpeed(f32 speed) {
		movementSpeed = speed;
	}

	void FPSCamController::setMouseSensitivity(f32 sensitivity) {
		mouseSensitivity = sensitivity;
	}

////////////////////////////////////////////////////////////// input

	void FPSCamController::handleContinuousInput(f32 deltatime) {
		f32 moveAmount = deltatime * movementSpeed;
		math::vec3 movementDir { 0, 0, 0 };
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
			[this](f32 xOffset, f32 yOffset) {
				handleMouseOffset(xOffset, yOffset);
			});
	}

	void FPSCamController::handleMouseOffset(f32 xOffset, 
											f32 yOffset) {
		camera->yaw(-xOffset * mouseSensitivity);
		camera->pitch(-yOffset * mouseSensitivity);
	}
}