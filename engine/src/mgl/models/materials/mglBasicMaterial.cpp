#include <mgl/models/materials/mglBasicMaterial.hpp>
#include <mgl/mglConventions.hpp>
#include <mgl/shaders/ShaderBuilder.hpp>

namespace mgl {

	void BasicMaterial::declareShaderUniforms(ShaderBuilder& shaders) {
		shaders.addUniform(COLOR_ATTRIBUTE);
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

	void BasicMaterial::setMaterialUniforms(ShaderProgram& shaders) {
		shaders.setUniform(COLOR_ATTRIBUTE, color);
	}

	math::vec3 BasicMaterial::getColor() {
		return color;
	}

}