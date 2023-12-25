#ifndef MGL_PHONG_MATERIAL_HPP
#define MGL_PHONG_MATERIAL_HPP

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
	PhongMaterial(const glm::vec3 &color);
	PhongMaterial(const glm::vec4& color);

	Material* setColor(const glm::vec3& color) override;
	Material* setColor(const glm::vec4& color) override;

	PhongMaterial* setAmbientColor(const glm::vec3& ambient);
	PhongMaterial* setDiffuseColor(const glm::vec3& diffuse);
	PhongMaterial* setSpecularColor(const glm::vec3& specular);

	PhongMaterial* setAmbientColor(const glm::vec4& ambient);
	PhongMaterial* setDiffuseColor(const glm::vec4& diffuse);
	PhongMaterial* setSpecularColor(const glm::vec4& specular);

	PhongMaterial* setShininess(GLfloat shininess);


private:
	glm::vec4 ambientColor  = glm::vec4(COLOR_WHITE, 1.0f);
	glm::vec4 diffuseColor  = glm::vec4(COLOR_WHITE, 1.0f);
	glm::vec4 specularColor = glm::vec4(COLOR_WHITE, 1.0f);
	GLfloat shininess		= 1.0f;

	void setMaterialUniforms(ShaderProgram* shaders) override;

};

}

#endif