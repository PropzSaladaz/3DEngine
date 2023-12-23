#include <mgl/scene/mglPointLight.hpp>
#include <glm/gtx/string_cast.hpp>
#include <logger.hpp>

namespace mgl {
	/////////////////////////////////////////////////////////////////// Constructor
	PointLight::PointLight(const SceneObject* position) : 
		PositionalLight(position) {}

	PointLight::PointLight(const glm::vec3& position) : PositionalLight(position) {}

	PointLight::PointLight(const glm::vec3& position, const glm::vec3& color) : 
		PositionalLight(position, color) {}

	PointLight::PointLight(const SceneObject* position, const glm::vec3& color) :
		PositionalLight(position, color) {}

	/////////////////////////////////////////////////////////////////// Shaders
	void PointLight::updateShaders(ShaderProgram* shader) {
		PositionalLight::updateShaders(shader);
		shader->setUniformInt(LIGHT_LIGHT_TYPE, POINT_LIGHT);
	}

}