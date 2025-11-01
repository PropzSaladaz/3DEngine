#pragma once

#include <mgl/models/materials/mglMaterial.hpp>


namespace mgl {

	class PhongMaterial : public Material {
	public:
		// shader must have a struct, with vec3 attributes, and float shininess
		static const char MATERIAL_PHONG_AMBIENT[];
		static const char MATERIAL_PHONG_DIFFUSE[];
		static const char MATERIAL_PHONG_SPECULAR[];
		static const char MATERIAL_PHONG_SHININESS[];

		static void declareShaderUniforms(ShaderProgram* shaders);

		PhongMaterial();
		PhongMaterial(const math::vec3 &color);
		PhongMaterial(const math::vec4& color);

		Material* setColor(const math::vec3& color) override;
		Material* setColor(const math::vec4& color) override;

		PhongMaterial* setAmbientColor(const math::vec3& ambient);
		PhongMaterial* setDiffuseColor(const math::vec3& diffuse);
		PhongMaterial* setSpecularColor(const math::vec3& specular);

		PhongMaterial* setAmbientColor(const math::vec4& ambient);
		PhongMaterial* setDiffuseColor(const math::vec4& diffuse);
		PhongMaterial* setSpecularColor(const math::vec4& specular);

		PhongMaterial* setShininess(GLfloat shininess);

	private:
		math::vec4 ambientColor  = math::vec4(COLOR_WHITE, 1.0f);
		math::vec4 diffuseColor  = math::vec4(COLOR_WHITE, 1.0f);
		math::vec4 specularColor = math::vec4(COLOR_WHITE, 1.0f);
		GLfloat shininess		= 1.0f;

		void setMaterialUniforms(ShaderProgram* shaders) override;

	};

}