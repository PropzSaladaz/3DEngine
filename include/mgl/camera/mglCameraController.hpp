#ifndef MGL_CAMERA_CONTROLLER_HPP
#define MGL_CAMERA_CONTROLLER_HPP

#include <mgl/mglSimulation.hpp>
#include <mgl/camera/mglCamera.hpp>

namespace mgl {

	class ICameraController : ITimeUpdateable {
	public:
		ICameraController(Camera* camera);
		void update(GLfloat deltaTime) override;
		void setCamera(Camera* camera);
		Camera* getCamera();

		void setActive();
		void setPosition(GLfloat x, GLfloat y, GLfloat z);

	protected:
		Camera* camera;
		void virtual handleContinuousInput(GLfloat deltatime) = 0;
		void virtual registerDiscreteInputHandler() = 0;
	};
}

#endif