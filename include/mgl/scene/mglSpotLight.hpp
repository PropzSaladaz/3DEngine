#ifndef MGL_SPOT_LIGHT_HPP
#define MGL_SPOT_LIGHT_HPP

#include <mgl/scene/mglPositionalLight.hpp>

namespace mgl {

class SpotLight : public PositionalLight {
public:
	SpotLight(glm::vec3 position);
	SpotLight(glm::vec3 position, glm::vec3 color);
	SpotLight(glm::vec3 position, glm::vec3 color, glm::vec3 direction);
	SpotLight(glm::vec3 position, glm::vec3 color, SceneObject* direction);

	void setDirection(glm::vec3 direction);
	void setDirection(SceneObject* direction);
	void setInnerCutoffAngle(GLfloat angleDegrees);
	void setOuterCutoffAngle(GLfloat angleDegrees);

	glm::vec3 getDirection();

	void updateShaders(ShaderProgram* shader) override;

private:
	SceneObject* direction;
	GLfloat spotOuterCosCutoff;
	GLfloat spotInnerCosCutoff;
	GLfloat epsilon;
};

}

#endif