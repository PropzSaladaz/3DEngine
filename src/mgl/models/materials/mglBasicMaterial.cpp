#include <mgl/models/materials/mglBasicMaterial.hpp>
#include <mgl/mglConventions.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace mgl {

	void BasicMaterial::declareShaderUniforms(ShaderProgram* shaders) {
		shaders->addUniform(COLOR_ATTRIBUTE);
	}

	BasicMaterial::BasicMaterial() {}
	BasicMaterial::BasicMaterial(const glm::vec3& color) {
		setColor(color);
	}
	Material* BasicMaterial::setColor(const glm::vec3& color) {
		this->color = color;
		return this;
	}

	void BasicMaterial::setMaterialUniforms(ShaderProgram* shaders) {
		shaders->setUniformVec3f(COLOR_ATTRIBUTE, glm::value_ptr(color));
	}

	glm::vec3 BasicMaterial::getColor() {
		return color;
	}

}