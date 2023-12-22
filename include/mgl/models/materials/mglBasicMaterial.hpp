#ifndef MGL_BASIC_MATERIAL_HPP
#define MGL_BASIC_MATERIAL_HPP

#include <mgl/models/materials/mglMaterial.hpp>

namespace mgl {

	class BasicMaterial : public Material {
	public:
		static void declareShaderUniforms(ShaderProgram* shaders);

		BasicMaterial();
		BasicMaterial(const glm::vec3& color);
		mgl::Material* setColor(const glm::vec3& color) override;
		glm::vec3 getColor();
	private:
		glm::vec3 color = COLOR_WHITE;

		void setMaterialUniforms(ShaderProgram* shaders) override;
	};

}

#endif