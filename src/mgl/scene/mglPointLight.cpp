#include <mgl/scene/mglPointLight.hpp>
#include <glm/gtx/string_cast.hpp>
#include <logger.hpp>

namespace mgl {
	/////////////////////////////////////////////////////////////////// Constructor
	PointLight::PointLight(const SceneObject* position) : Light() {
		this->position = position;
		setColor(COLOR_WHITE);
	}

	PointLight::PointLight(const glm::vec3& position) {
		setPosition(position);
	}

	PointLight::PointLight(const glm::vec3& position, const glm::vec3& color) {
		setPosition(position);
		setColor(color);
	}

	PointLight::PointLight(const SceneObject* position, const glm::vec3& color) :
		Light() {
		this->position = position;
		setColor(color);
	}


	/////////////////////////////////////////////////////////////////// Setters
	void PointLight::setColor(const glm::vec3& color){
		setAmbient(color);
		setDiffuse(color);
		setSpecular(color);
	}

	void PointLight::setPosition(const glm::vec3& position) {
		mgl::SceneObject* lightPos = new SceneObject();
		lightPos->setPosition(position);
		this->position = lightPos;
	}

	void PointLight::setAmbient(const glm::vec3& ambient){
		ambientColor = ambient;
	}
	void PointLight::setDiffuse(const glm::vec3& diffuse){
		diffuseColor = diffuse;
	}
	void PointLight::setSpecular(const glm::vec3& specular){
		specularColor = specular;
	}

	/////////////////////////////////////////////////////////////////// Getters

	glm::vec3 PointLight::getPosition() {
		return position->getAbsolutePosition();
	}

	glm::vec3 PointLight::getAmbient(){
		return ambientColor;
	}
	glm::vec3 PointLight::getDiffuse(){
		return diffuseColor;
	}
	glm::vec3 PointLight::getSpecular() {
		return specularColor;
	}

	/////////////////////////////////////////////////////////////////// Shaders
	void PointLight::updateShaders(ShaderProgram* shader) {
		glm::vec3 eyeSpaceAbsolutePos = camera->getViewMatrix() * glm::vec4(position->getAbsolutePosition(), 1.0);
		shader->setUniformVec3f(LIGHT_POSITION, glm::value_ptr(eyeSpaceAbsolutePos));
		shader->setUniformVec3f(LIGHT_AMBIENT, glm::value_ptr(ambientColor));
		shader->setUniformVec3f(LIGHT_DIFFUSE, glm::value_ptr(diffuseColor));
		shader->setUniformVec3f(LIGHT_SPECULAR, glm::value_ptr(specularColor));
	}

}