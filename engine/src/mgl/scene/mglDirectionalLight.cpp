#include <mgl/scene/mglDirectionalLight.hpp>

namespace mgl {

	DirectionalLight::DirectionalLight(const math::vec3& direction) : Light() {
		setDirection(direction);
	}

	DirectionalLight::DirectionalLight(const math::vec3& direction, 
		const math::vec3& color) : DirectionalLight(direction) {
		setColor(color);
	}

	void DirectionalLight::setDirection(const math::vec3& direction) {
		this->direction = direction;
	}
	math::vec3 DirectionalLight::getDirection() const {
		return direction;
	}

	void DirectionalLight::updateShaders(ShaderProgram* shader) {
		math::vec3 WorldSpaceDirection = direction;
		shader->setUniformBool(LIGHT_IS_ENABLED, enabled);
		shader->setUniformInt  (LIGHT_LIGHT_TYPE, DIRECTIONAL_LIGHT);
		shader->setUniformVec3f(LIGHT_DIRECTION,  WorldSpaceDirection.data());
		shader->setUniformVec4f(LIGHT_AMBIENT, ambientColor.data());
		shader->setUniformVec4f(LIGHT_DIFFUSE, diffuseColor.data());
		shader->setUniformVec4f(LIGHT_SPECULAR, specularColor.data());
	}

}