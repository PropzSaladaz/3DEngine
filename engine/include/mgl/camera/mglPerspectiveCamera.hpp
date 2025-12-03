#ifndef MGL_PERSPECTIVE_CAMERA_HPP
#define MGL_PERSPECTIVE_CAMERA_HPP

#include <mgl/camera/mglCamera.hpp>
#include "types.hpp"

namespace mgl {

	class PerspectiveCamera : public Camera {
	public:
		PerspectiveCamera(ui32 bindingPoint, const PerspectiveParams& params);

		void setPerspective(const PerspectiveParams& params);
		void setPerspective(f32 fov, f32 screen_ratio,
			f32 near, f32 far);

		void updateScreenRatio(i32 width, i32 height) override;
		void computeProjectionMatrix() override;
		void zoom(f32 step) override;

	private:
		PerspectiveParams perspectiveParams{};
	};


}

#endif
