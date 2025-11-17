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

	void DirectionalLight::updateShaders(ShaderProgram& shader) {
		math::vec3 WorldSpaceDirection = direction;
		shader.setUniform(LIGHT_IS_ENABLED, enabled);
		shader.setUniform(LIGHT_LIGHT_TYPE, DIRECTIONAL_LIGHT);
		shader.setUniform(LIGHT_DIRECTION,  WorldSpaceDirection);
		shader.setUniform(LIGHT_AMBIENT, ambientColor);
		shader.setUniform(LIGHT_DIFFUSE, diffuseColor);
		shader.setUniform(LIGHT_SPECULAR, specularColor);
	}

}