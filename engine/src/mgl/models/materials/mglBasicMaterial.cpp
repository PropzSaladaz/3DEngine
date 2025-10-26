#include <mgl/models/materials/mglBasicMaterial.hpp>
#include <mgl/mglConventions.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace mgl {

	void BasicMaterial::declareShaderUniforms(ShaderProgram* shaders) {
		shaders->addUniform(COLOR_ATTRIBUTE);
	}

	BasicMaterial::BasicMaterial() {}
	BasicMaterial::BasicMaterial(const glm::vec3& color) 
		: BasicMaterial(glm::vec4(color, 1.0f)) {}
	BasicMaterial::BasicMaterial(const glm::vec4& color) {
		setColor(color);
	}

	mgl::Material* BasicMaterial::setColor(const glm::vec4& color) {
		this->color = color;
		return this;
	}

	Material* BasicMaterial::setColor(const glm::vec3& color) {
		return setColor(glm::vec4(color, 1.0f));
	}

	void BasicMaterial::setMaterialUniforms(ShaderProgram* shaders) {
		shaders->setUniformVec4f(COLOR_ATTRIBUTE, glm::value_ptr(color));
	}

	glm::vec3 BasicMaterial::getColor() {
		return color;
	}

}