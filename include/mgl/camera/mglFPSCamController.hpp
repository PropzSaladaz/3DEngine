#ifndef MGL_FPS_CAMERA_HPP
#define MGL_FPS_CAMERA_HPP

#include <mgl/camera/mglCameraController.hpp>

#define DEFAULT_FPS_MOVEMENT_SPEED 4.0f
#define DEFAULT_FPS_MOUSE_SENSITIVITY 4.0f

namespace mgl {

	class FPSCamController : public ICameraController {
	public:
		FPSCamController(Camera* camera);
		FPSCamController(Camera* camera, GLfloat speed, GLfloat sensitivity);

		void setMovementSpeed(GLfloat speed);
		void setMouseSensitivity(GLfloat sensitivity);

	protected:
		void handleContinuousInput(GLfloat deltatime) override;
		void registerDiscreteInputHandler() override;
	private:
		GLfloat movementSpeed = DEFAULT_FPS_MOVEMENT_SPEED;
		GLfloat mouseSensitivity = DEFAULT_FPS_MOUSE_SENSITIVITY;

		void handleMouseOffset(GLfloat xOffset, GLfloat yOffset);
	};

}

#endif