#ifndef MGL_PHONG_MATERIAL_HPP
#define MGL_PHONG_MATERIAL_HPP

#include <mgl/models/materials/mglMaterial.hpp>


namespace mgl {


class PhongMaterial : public Material {
public:
	// shader uniform values
	static inline const char MATERIAL_PHONG_AMBIENT[] = "material.ambient";
	static inline const char MATERIAL_PHONG_DIFFUSE[] = "material.diffuse";
	static inline const char MATERIAL_PHONG_SPECULAR[] = "material.specular";
	static inline const char MATERIAL_PHONG_SHININESS[] = "material.shininess";

	PhongMaterial(ShaderProgram* shaders);
	PhongMaterial(ShaderProgram* shaders, const glm::vec3 &color);

	Material* setColor(const glm::vec3& color) override;
	PhongMaterial* setAmbientColor(const glm::vec3& ambient);
	PhongMaterial* setDiffuseColor(const glm::vec3& diffuse);
	PhongMaterial* setSpecularColor(const glm::vec3& specular);
	PhongMaterial* setShininess(GLfloat shininess);

private:
	glm::vec3 ambientColor  = COLOR_WHITE;
	glm::vec3 diffuseColor  = COLOR_WHITE;
	glm::vec3 specularColor = COLOR_WHITE;
	GLfloat shininess		= 1.0f;

	void setMaterialUniforms(ShaderProgram* shaders) override;
	std::string getShaderPrefix() override;

};

}

#endif