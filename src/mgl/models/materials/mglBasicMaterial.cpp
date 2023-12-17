#include <mgl/models/materials/mglBasicMaterial.hpp>
#include <mgl/mglConventions.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace mgl {

	BasicMaterial::BasicMaterial(ShaderProgram* shaders) : Material(shaders) {}
	BasicMaterial::BasicMaterial(ShaderProgram* shaders, 
		const glm::vec3& color) : BasicMaterial(shaders) {

	}
	Material* BasicMaterial::setColor(const glm::vec3& color) {
		this->color = color;
		return this;
	}

	void BasicMaterial::setMaterialUniforms(ShaderProgram* shaders) {
		shaders->setUniformVec3f(COLOR_ATTRIBUTE, glm::value_ptr(color));
	}

	std::string BasicMaterial::getShaderPrefix()
	{
		return "Basic";
	}

	glm::vec3 BasicMaterial::getColor() {
		return color;
	}

}