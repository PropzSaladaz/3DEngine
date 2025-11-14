#pragma once

#include <mgl/models/materials/mglMaterial.hpp>

namespace mgl {

	class BasicMaterial : public Material {
	public:
		static void declareShaderUniforms(ShaderBuilder& shaders);

		BasicMaterial();
		BasicMaterial(const math::vec3& color);
		BasicMaterial(const math::vec4& color);

		mgl::Material* setColor(const math::vec3& color) override;
		mgl::Material* setColor(const math::vec4& color) override;

		math::vec3 getColor();
	private:
		math::vec4 color = math::vec4(COLOR_WHITE, 1.0f);

		void setMaterialUniforms(ShaderProgram& shaders) override;
	};

}