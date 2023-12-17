#ifndef MGL_BASIC_MATERIAL_HPP
#define MGL_BASIC_MATERIAL_HPP

#include <mgl/models/materials/mglMaterial.hpp>

namespace mgl {

	class BasicMaterial : public Material {
	public:
		BasicMaterial(ShaderProgram* shaders);
		BasicMaterial(ShaderProgram* shaders, const glm::vec3& color);
		mgl::Material* setColor(const glm::vec3& color) override;
		glm::vec3 getColor();
	private:
		glm::vec3 color = COLOR_WHITE;

		void setMaterialUniforms(ShaderProgram* shaders) override;


		// Inherited via Material
		std::string getShaderPrefix() override;

	};

}

#endif