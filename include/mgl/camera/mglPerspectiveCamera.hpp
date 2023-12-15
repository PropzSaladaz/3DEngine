#ifndef MGL_PERSPECTIVE_CAMERA_HPP
#define MGL_PERSPECTIVE_CAMERA_HPP

#include <glm/glm.hpp>
#include <mgl/camera/mglCamera.hpp>

namespace mgl {

	class PerspectiveCamera : public Camera {
	public:
		PerspectiveCamera(GLuint bindingPoint, PerspectiveParams* params);

		void setPerspective(PerspectiveParams* params);
		void setPerspective(GLfloat fov, GLfloat screen_ratio,
			GLfloat near, GLfloat far);

		void updateScreenRatio(GLuint width, GLuint height) override;
		void computeProjectionMatrix() override;
		void zoom(GLfloat step) override;

	private:
		PerspectiveParams* perspectiveParams;
	};


}

#endif