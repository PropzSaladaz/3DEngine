#ifndef MGL_ANIMATION_HPP
#define MGL_ANIMATION_HPP

#include "types.hpp"
#include <memory>

#include <mgl/mglSimulation.hpp>
#include <mgl/mglTransform.hpp>

namespace mgl {

class Animation {
public:
	Animation(Transform* origin, const Transform* target);
	void step(f32 step);
	void setSpeed(f32 speed);
private:
	std::unique_ptr<Transform> originTransform;
	Transform* currentTransform;
	const Transform* targetTransform;
	f32 interpolationSpeed = 0.05f;
	f32 currentStep = 0.0f;

	bool interpolatePos;
	bool interpolateScale;
	bool interpolateRot;
};

} // namespace mgl

#endif
