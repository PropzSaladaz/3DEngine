#ifndef MGL_ANIMATION_HPP
#define MGL_ANIMATION_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <mgl/mglSimulation.hpp>
#include <mgl/mglTransform.hpp>

namespace mgl {

class Animation {
public:
	Animation(Transform* origin, const Transform* target);
	void step(GLfloat step);
	void setSpeed(GLfloat speed);
private:
	const Transform* originTransform;
	Transform* currentTransform;
	const Transform* targetTransform;
	GLfloat interpolationSpeed = 0.05f;
	GLfloat currentStep = 0.0f;

	bool interpolatePos;
	bool interpolateScale;
	bool interpolateRot;
};

} // namespace mgl

#endif