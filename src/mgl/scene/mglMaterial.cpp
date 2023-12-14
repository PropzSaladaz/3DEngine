#include <mgl/scene/mglMaterial.hpp>
#include <mgl/mglShaders.hpp>
#include <mgl/mglConventions.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace mgl {
	Material::Material(ShaderProgram* shaders) {
		Shaders = shaders;
	}

	void Material::bind() {
		Shaders->bind();
		// set color
		Shaders->setUniformVec4f(COLOR_ATTRIBUTE, glm::value_ptr(color));
	}

	void Material::unbind() {
		Shaders->unbind();
	}

	Material* Material::setColor(const glm::vec3& _color) {
		color = _color;
		return this;
	}

}