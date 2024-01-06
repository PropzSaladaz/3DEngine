#include <mgl/camera/mglCameraController.hpp>
#include <mgl/mglSimulation.hpp>
#include <utils/logger.hpp>

namespace mgl {
	ICameraController::ICameraController(Camera* camera) {
		setCamera(camera);
		Simulation::getInstance().trackUpdateable(getId(), this);
	}
	void ICameraController::update(GLfloat deltaTime) {
		if (isActive) {
			handleContinuousInput(deltaTime);
		}
	} 
	void ICameraController::setCamera(Camera* camera) {
		this->camera = camera; 
	}
	Camera* ICameraController::getCamera() {
		return camera;
	}

	void ICameraController::setActive() {
#ifdef DEBUG
		if (camera == nullptr) {
			util::Logger::LogError("Camera is null");
			exit(EXIT_FAILURE);
		}
#endif
		camera->setActive();
		isActive = true;
	}


	void ICameraController::setPosition(GLfloat x, GLfloat y, GLfloat z) {
#ifdef DEBUG
		if (camera == nullptr) {
			util::Logger::LogError("Camera is null");
			exit(EXIT_FAILURE);
		}
#endif
		camera->setPosition(x, y, z);
	}

	void ICameraController::unset() {
		isActive = false;
	}

}