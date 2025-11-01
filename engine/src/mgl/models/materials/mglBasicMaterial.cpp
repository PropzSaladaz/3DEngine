#include <mgl/models/materials/mglBasicMaterial.hpp>
#include <mgl/mglConventions.hpp>

namespace mgl {

	void BasicMaterial::declareShaderUniforms(ShaderProgram* shaders) {
		shaders->addUniform(COLOR_ATTRIBUTE);
	}

	BasicMaterial::BasicMaterial() {}
	BasicMaterial::BasicMaterial(const math::vec3& color) 
		: BasicMaterial(math::vec4(color, 1.0f)) {}
	BasicMaterial::BasicMaterial(const math::vec4& color) {
		setColor(color);
	}

	mgl::Material* BasicMaterial::setColor(const math::vec4& color) {
		this->color = color;
		return this;
	}

	Material* BasicMaterial::setColor(const math::vec3& color) {
		return setColor(math::vec4(color, 1.0f));
	}

	void BasicMaterial::setMaterialUniforms(ShaderProgram* shaders) {
		shaders->setUniformVec4f(COLOR_ATTRIBUTE, color.data());
	}

	math::vec3 BasicMaterial::getColor() {
		return color;
	}

}