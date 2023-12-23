#include <mgl/scene/mglDirectionalLight.hpp>

namespace mgl {

	DirectionalLight::DirectionalLight(const glm::vec3& direction) : Light() {
		setDirection(direction);
	}

	DirectionalLight::DirectionalLight(const glm::vec3& direction, 
		const glm::vec3& color) : DirectionalLight(direction) {
		setColor(color);
	}

	void DirectionalLight::setDirection(const glm::vec3& direction) {
		this->direction = direction;
	}
	glm::vec3 DirectionalLight::getDirection() const {
		return direction;
	}

	void DirectionalLight::updateShaders(ShaderProgram* shader) {
		glm::vec3 eyeSpaceDirection = camera->getViewMatrix() * glm::vec4(direction , 0.0);
		shader->setUniformBool(LIGHT_IS_ENABLED, enabled);
		shader->setUniformInt  (LIGHT_LIGHT_TYPE, DIRECTIONAL_LIGHT);
		shader->setUniformVec3f(LIGHT_DIRECTION,  glm::value_ptr(eyeSpaceDirection));
		shader->setUniformVec3f(LIGHT_AMBIENT, glm::value_ptr(ambientColor));
		shader->setUniformVec3f(LIGHT_DIFFUSE, glm::value_ptr(diffuseColor));
		shader->setUniformVec3f(LIGHT_SPECULAR, glm::value_ptr(specularColor));
	}

}