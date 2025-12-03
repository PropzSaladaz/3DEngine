#include <mgl/camera/mglCamera.hpp>
#include <mgl/camera/mglPerspectiveCamera.hpp>

namespace mgl {

	PerspectiveCamera::PerspectiveCamera(ui32 bindingPoint, 
					const PerspectiveParams& params) : Camera(bindingPoint) {

		updateViewMatrix();
		setPerspective(params);
	}

	void PerspectiveCamera::setPerspective(const PerspectiveParams& params) {
		perspectiveParams = params;
		updateProjectionMatrix();
	}

	void PerspectiveCamera::setPerspective(GLfloat fov, GLfloat screen_ratio,
		GLfloat near, GLfloat far) {
		setPerspective(PerspectiveParams{ fov, screen_ratio, near, far });
	}

	void PerspectiveCamera::updateScreenRatio(i32 width, i32 height) {
		Camera::perspectiveResize(&perspectiveParams, width, height);
		setPerspective(perspectiveParams);
	}

	void PerspectiveCamera::computeProjectionMatrix() {
		projectionMatrix = math::perspective(
			math::radians(perspectiveParams.fov),
			perspectiveParams.screenRatio,
			perspectiveParams.near,
			perspectiveParams.far);
	}

	void PerspectiveCamera::zoom(f32 step) {
		translate(getFrontV() * step);
		updateViewMatrix();
	}
};
