#pragma once

#include <mgl/mglSimulation.hpp>
#include <mgl/camera/mglCamera.hpp>

namespace mgl {

	class ICameraController : ITimeUpdateable {
	public:
		ICameraController(Camera* camera);
		void update(f32 deltaTime) override;
		void setCamera(Camera* camera);
		Camera* getCamera();

		void setActive();
		void unset();
		void setPosition(f32 x, f32 y, f32 z);

	protected:
		bool isActive = false;
		Camera* camera;
		void virtual handleContinuousInput(f32 deltatime) = 0;
		void virtual registerDiscreteInputHandler() = 0;
	};
}