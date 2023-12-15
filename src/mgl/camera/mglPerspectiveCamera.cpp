#include <mgl/camera/mglCamera.hpp>
#include <mgl/camera/mglPerspectiveCamera.hpp>

namespace mgl {

	PerspectiveCamera::PerspectiveCamera(GLuint bindingPoint, 
					PerspectiveParams* params) : Camera(bindingPoint) {

		updateViewMatrix();
		setPerspective(params);
	}

	void PerspectiveCamera::setPerspective(PerspectiveParams* params) {
		perspectiveParams = params;
		updateProjectionMatrix();
	}

	void PerspectiveCamera::setPerspective(GLfloat fov, GLfloat screen_ratio,
		GLfloat near, GLfloat far) {
		perspectiveParams = new PerspectiveParams{ fov, screen_ratio, near, far };
		updateProjectionMatrix();
	}

	void PerspectiveCamera::updateScreenRatio(GLuint width, GLuint height) {
		Camera::perspectiveResize(perspectiveParams, width, height);
		setPerspective(perspectiveParams);
	}

	void PerspectiveCamera::computeProjectionMatrix() {
		projectionMatrix = glm::perspective(
			glm::radians(perspectiveParams->fov),
			perspectiveParams->screenRatio,
			perspectiveParams->near,
			perspectiveParams->far);
	}

	void PerspectiveCamera::zoom(GLfloat step) {
		translate(getFrontV() * step);
		updateViewMatrix();
	}
};