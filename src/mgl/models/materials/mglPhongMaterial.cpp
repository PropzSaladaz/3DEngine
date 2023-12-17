#include <mgl/models/materials/mglPhongMaterial.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace mgl {
	PhongMaterial::PhongMaterial(ShaderProgram* shaders) 
		: Material(shaders) {}

	PhongMaterial::PhongMaterial(ShaderProgram* shaders, 
		const glm::vec3& color) : PhongMaterial(shaders) {
		setColor(color);
	}

	Material* PhongMaterial::setColor(const glm::vec3& color) {
		setAmbientColor(color);
		setDiffuseColor(color);
		setSpecularColor(color);
		return this;
	}
	PhongMaterial* PhongMaterial::setAmbientColor(const glm::vec3& ambient) {
		ambientColor = ambient;
		return this;
	}
	PhongMaterial* PhongMaterial::setDiffuseColor(const glm::vec3& diffuse) {
		diffuseColor = diffuse;
		return this;
	}
	PhongMaterial* PhongMaterial::setSpecularColor(const glm::vec3& specular) {
		specularColor = specular;
		return this;
	}
	PhongMaterial* PhongMaterial::setShininess(GLfloat shininess) {
		this->shininess = shininess;
		return this;
	}

	std::string PhongMaterial::getShaderPrefix() {
		return "Phong";
	}

	void PhongMaterial::setMaterialUniforms(ShaderProgram* shaders) {
		shaders->setUniformVec3f(MATERIAL_PHONG_AMBIENT, glm::value_ptr(ambientColor));
		shaders->setUniformVec3f(MATERIAL_PHONG_DIFFUSE, glm::value_ptr(diffuseColor));
		shaders->setUniformVec3f(MATERIAL_PHONG_SPECULAR, glm::value_ptr(specularColor));
		shaders->setUniformFloat(MATERIAL_PHONG_SHININESS, shininess);
	}

}