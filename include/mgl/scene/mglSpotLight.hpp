#ifndef MGL_SPOT_LIGHT_HPP
#define MGL_SPOT_LIGHT_HPP

#include <mgl/scene/mglPositionalLight.hpp>

namespace mgl {

class SpotLight : public PositionalLight {

public:
	static const GLfloat DEFAULT_INNER_COS_CUTOFF;
	static const GLfloat DEFAULT_OUTER_COS_CUTOFF;
	static const GLfloat DEFAULT_EPSILON;

	SpotLight();
	SpotLight(const glm::vec3 &position);
	SpotLight(const SceneObject* position);
	SpotLight(const glm::vec3 &position, const glm::vec3 &color);
	SpotLight(const SceneObject* position, const glm::vec3 &color);
	SpotLight(const glm::vec3 &position, const glm::vec3 &color, const glm::vec3 &target);
	SpotLight(const glm::vec3 &position, const glm::vec3 &color, const SceneObject* target);
	SpotLight(const SceneObject* position, const glm::vec3 &color, const glm::vec3 & target);
	SpotLight(const SceneObject* position, const glm::vec3 &color, const SceneObject* target);

	void setTarget(const glm::vec3 &target);
	void setTarget(const SceneObject* target);
	void setInnerCutoffAngle(GLfloat angleDegrees);
	void setOuterCutoffAngle(GLfloat angleDegrees);

	glm::vec3 getDirection();

	void updateShaders(ShaderProgram* shader) override;

private:
	const SceneObject* target = nullptr;
	GLfloat spotOuterCosCutoff = DEFAULT_OUTER_COS_CUTOFF;
	GLfloat spotInnerCosCutoff = DEFAULT_INNER_COS_CUTOFF;
	GLfloat epsilon = DEFAULT_EPSILON;
};

}

#endif