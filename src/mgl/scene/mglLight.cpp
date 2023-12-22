#include <mgl/scene/mglLight.hpp>
#include <mgl/camera/mglCamera.hpp>

namespace mgl {
	const char Light::LIGHT_POSITION[] = "light.position";
	const char Light::LIGHT_AMBIENT[] = "light.ambient";
	const char Light::LIGHT_DIFFUSE[] = "light.diffuse";
	const char Light::LIGHT_SPECULAR[] = "light.specular";

	void Light::declareShaderUniforms(ShaderProgram* shaders) {
		shaders->addUniform(LIGHT_POSITION);
		shaders->addUniform(LIGHT_AMBIENT);
		shaders->addUniform(LIGHT_DIFFUSE);
		shaders->addUniform(LIGHT_SPECULAR);
	}

	Light::Light() {
		enabled = true;
	}
	Light::~Light() {}
	void Light::enable() {
		enabled = true;
	}
	void Light::disable() {
		enabled = false;
	}
	bool Light::isEnabled() {
		return enabled;
	}

	void Light::assignCamera(const Camera* camera) {
		this->camera = camera;
	}
}