#include <mgl/mglAnimation.hpp>

namespace mgl {


Animation::Animation(Transform* origin, const Transform* target) {
	originTransform = std::make_unique<Transform>(origin); // shallow copy
	currentTransform = origin;
	targetTransform = target;

	// check which transform dimensions will need to be updated
	interpolatePos = !math::equal(
		originTransform->getPosition(),
		targetTransform->getPosition(),
		mgl::math::epsilon<float>());

	interpolateScale = !math::equal(
		originTransform->getScale(),
		targetTransform->getScale(),
		mgl::math::epsilon<float>());

	interpolateRot = !math::equal(
		originTransform->getRotationQuat(),
		targetTransform->getRotationQuat(),
		mgl::math::epsilon<float>());
}

void Animation::step(f32 step) {
	if (currentStep >= 0 && currentStep <= 1) currentStep += step * interpolationSpeed;

	if (currentStep < 0) currentStep = 0.0f;
	else if (currentStep > 1) currentStep = 1.0f;

	if (interpolatePos) {
		currentTransform->setPosition(math::lerp(
			originTransform->getPosition(),
			targetTransform->getPosition(),
			currentStep));
	}
	if (interpolateScale) {
		currentTransform->setScale(math::lerp(
			originTransform->getScale(),
			targetTransform->getScale(),
			currentStep));
	}
	if (interpolateRot) {
		currentTransform->setRotationQuat(math::slerp(
			originTransform->getRotationQuat(),
			targetTransform->getRotationQuat(),
			currentStep));
	}
}

void Animation::setSpeed(GLfloat speed) {
	interpolationSpeed = speed;
}
} // namespace mgl
