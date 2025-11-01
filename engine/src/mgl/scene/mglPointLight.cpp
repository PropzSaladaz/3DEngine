#include <mgl/scene/mglPointLight.hpp>
namespace mgl {
	/////////////////////////////////////////////////////////////////// Constructor
	PointLight::PointLight(const SceneObject* position) : 
		PositionalLight(position) {}

	PointLight::PointLight(const math::vec3& position) : PositionalLight(position) {}

	PointLight::PointLight(const math::vec3& position, const math::vec3& color) : 
		PositionalLight(position, color) {}

	PointLight::PointLight(const SceneObject* position, const math::vec3& color) :
		PositionalLight(position, color) {}

	/////////////////////////////////////////////////////////////////// Shaders
	void PointLight::updateShaders(ShaderProgram* shader) {
		PositionalLight::updateShaders(shader);
		shader->setUniformInt(LIGHT_LIGHT_TYPE, POINT_LIGHT);
	}

}