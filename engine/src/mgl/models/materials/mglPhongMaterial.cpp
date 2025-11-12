#include <mgl/models/materials/mglPhongMaterial.hpp>

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

	PhongMaterial::PhongMaterial(const math::vec3& color) 
	: PhongMaterial(math::vec4(color, 1.0f)) {}

	PhongMaterial::PhongMaterial(const math::vec4& color) {
		setColor(color);
	}

	Material* PhongMaterial::setColor(const math::vec3& color) {
		math::vec4 col = math::vec4(color, 1.0f);
		setColor(col);
		return this;
	}

	Material* PhongMaterial::setColor(const math::vec4& color) {
		setAmbientColor(color);
		setDiffuseColor(color);
		setSpecularColor(color);
		return this;
	}

	PhongMaterial* PhongMaterial::setAmbientColor(const math::vec3& ambient) {
		return setAmbientColor(math::vec4(ambient, 1.0f));
	}
	PhongMaterial* PhongMaterial::setDiffuseColor(const math::vec3& diffuse) {
		return setDiffuseColor(math::vec4(diffuse, 1.0f));
	}
	PhongMaterial* PhongMaterial::setSpecularColor(const math::vec3& specular) {
		return setSpecularColor(math::vec4(specular, 1.0f));
	}

	PhongMaterial* PhongMaterial::setAmbientColor(const math::vec4& ambient) {
		ambientColor = ambient;
		return this;
	}
	PhongMaterial* PhongMaterial::setDiffuseColor(const math::vec4& diffuse) {
		diffuseColor = diffuse;
		return this;
	}
	PhongMaterial* PhongMaterial::setSpecularColor(const math::vec4& specular) {
		specularColor = specular;
		return this;
	}

	PhongMaterial* PhongMaterial::setShininess(GLfloat shininess) {
		this->shininess = shininess;
		return this;
	}

	void PhongMaterial::setMaterialUniforms(ShaderProgram* shaders) {
		shaders->setUniform(MATERIAL_PHONG_AMBIENT,  ambientColor);
		shaders->setUniform(MATERIAL_PHONG_DIFFUSE,  diffuseColor);
		shaders->setUniform(MATERIAL_PHONG_SPECULAR, specularColor);
		shaders->setUniform(MATERIAL_PHONG_SHININESS, shininess);
	}

}