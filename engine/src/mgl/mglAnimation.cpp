#include <mgl/mglAnimation.hpp>
#include <iostream>

#define THRESHOLD_FLOAT static_cast<float>(1.0e-5)

namespace mgl {


Animation::Animation(Transform* origin, const Transform* target) {
	originTransform = new Transform(origin); // shallow copy
	currentTransform = origin;
	targetTransform = target;

	// check which transform dimensions will need to be updated
	interpolatePos = !glm::all(glm::equal(
		originTransform->getPosition(),
		targetTransform->getPosition(),
		THRESHOLD_FLOAT));

	interpolateScale = !glm::all(glm::equal(
		originTransform->getScale(),
		targetTransform->getScale(),
		THRESHOLD_FLOAT));

	interpolateRot = !glm::all(glm::equal(
		originTransform->getRotationQuat(),
		targetTransform->getRotationQuat(),
		THRESHOLD_FLOAT));
}

void Animation::step(GLfloat step) {
	if (currentStep >= 0 && currentStep <= 1) currentStep += step * interpolationSpeed;

	if (currentStep < 0) currentStep = 0.0f;
	else if (currentStep > 1) currentStep = 1.0f;

	if (interpolatePos) {
		currentTransform->setPosition(glm::mix(
			originTransform->getPosition(),
			targetTransform->getPosition(),
			currentStep));
	}
	if (interpolateScale) {
		currentTransform->setScale(glm::mix(
			originTransform->getScale(),
			targetTransform->getScale(),
			currentStep));
	}
	if (interpolateRot) {
		currentTransform->setRotationQuat(glm::slerp(
			originTransform->getRotationQuat(),
			targetTransform->getRotationQuat(),
			currentStep));
	}
}

void Animation::setSpeed(GLfloat speed) {
	interpolationSpeed = speed;
}
} // namespace mgl