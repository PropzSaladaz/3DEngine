#include <mgl/models/materials/mglPhongMaterial.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace mgl {
	const char PhongMaterial::MATERIAL_PHONG_AMBIENT[] = "material.ambient";
	const char PhongMaterial::MATERIAL_PHONG_DIFFUSE[] = "material.diffuse";
	const char PhongMaterial::MATERIAL_PHONG_SPECULAR[] = "material.specular";
	const char PhongMaterial::MATERIAL_PHONG_SHININESS[] = "material.shininess";

	void PhongMaterial::declareShaderUniforms(ShaderProgram* shaders) {
		shaders->addUniform(MATERIAL_PHONG_AMBIENT);
		shaders->addUniform(MATERIAL_PHONG_DIFFUSE);
		shaders->addUniform(MATERIAL_PHONG_SPECULAR);
		shaders->addUniform(MATERIAL_PHONG_SHININESS);
	}

	PhongMaterial::PhongMaterial() {}

	PhongMaterial::PhongMaterial(const glm::vec3& color) 
	: PhongMaterial(glm::vec4(color, 1.0f)) {}

	PhongMaterial::PhongMaterial(const glm::vec4& color) {
		setColor(color);
	}

	Material* PhongMaterial::setColor(const glm::vec3& color) {
		glm::vec4 col = glm::vec4(color, 1.0f);
		setColor(col);
		return this;
	}

	Material* PhongMaterial::setColor(const glm::vec4& color) {
		setAmbientColor(color);
		setDiffuseColor(color);
		setSpecularColor(color);
		return this;
	}

	PhongMaterial* PhongMaterial::setAmbientColor(const glm::vec3& ambient) {
		return setAmbientColor(glm::vec4(ambient, 1.0f));
	}
	PhongMaterial* PhongMaterial::setDiffuseColor(const glm::vec3& diffuse) {
		return setDiffuseColor(glm::vec4(diffuse, 1.0f));
	}
	PhongMaterial* PhongMaterial::setSpecularColor(const glm::vec3& specular) {
		return setSpecularColor(glm::vec4(specular, 1.0f));
	}

	PhongMaterial* PhongMaterial::setAmbientColor(const glm::vec4& ambient) {
		ambientColor = ambient;
		return this;
	}
	PhongMaterial* PhongMaterial::setDiffuseColor(const glm::vec4& diffuse) {
		diffuseColor = diffuse;
		return this;
	}
	PhongMaterial* PhongMaterial::setSpecularColor(const glm::vec4& specular) {
		specularColor = specular;
		return this;
	}

	PhongMaterial* PhongMaterial::setShininess(GLfloat shininess) {
		this->shininess = shininess;
		return this;
	}

	void PhongMaterial::setMaterialUniforms(ShaderProgram* shaders) {
		shaders->setUniformVec4f(MATERIAL_PHONG_AMBIENT, glm::value_ptr(ambientColor));
		shaders->setUniformVec4f(MATERIAL_PHONG_DIFFUSE, glm::value_ptr(diffuseColor));
		shaders->setUniformVec4f(MATERIAL_PHONG_SPECULAR, glm::value_ptr(specularColor));
		shaders->setUniformFloat(MATERIAL_PHONG_SHININESS, shininess);
	}

}