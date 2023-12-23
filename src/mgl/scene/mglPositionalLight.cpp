#include <mgl/scene/mglPositionalLight.hpp>
#include <glm/gtx/string_cast.hpp>
#include <logger.hpp>

namespace mgl {
	const GLfloat PositionalLight::DEFAULT_ATTENUATION = 200.0f;

	/////////////////////////////////////////////////////////////////// Constructor
	PositionalLight::PositionalLight() : Light() {
		setColor(COLOR_WHITE);
		setAttenuation(DEFAULT_ATTENUATION);
	}
	PositionalLight::PositionalLight(const SceneObject* position) 
		: PositionalLight() {
		setPosition(position);
	}

	PositionalLight::PositionalLight(const glm::vec3& position) 
		: PositionalLight() {
		setPosition(position);
	}

	PositionalLight::PositionalLight(const glm::vec3& position, 
		const glm::vec3& color) : PositionalLight(position) {
		setColor(color);
	}

	PositionalLight::PositionalLight(const SceneObject* position, 
		const glm::vec3& color) : PositionalLight(position) {
		setColor(color);
	}

	/////////////////////////////////////////////////////////////////// Setters

	void PositionalLight::setPosition(const glm::vec3& position) {
		mgl::SceneObject* lightPos = new SceneObject();
		lightPos->setPosition(position);
		this->position = lightPos;
	}

	void PositionalLight::setPosition(const SceneObject* position) {
		this->position = position;
	}

	void PositionalLight::setAttenuation(GLuint distance) {
		GLuint lower = attenuationDistances[0];

		if (distance < lower) {
			util::Logger::LogError("Attenuation distance must be >= " + lower);
			exit(EXIT_FAILURE);
		};

		for (GLuint attDist : attenuationDistances) {
			if (distance > attDist) {
				lower = attDist;
			}
			else break;
		}
		auto it = defaultAttenuations.find(lower);
		attenuation = it->second;
	}

	void PositionalLight::setAttenuation(const LightAttenuation& attenuation) {
		this->attenuation = attenuation;
	}

	/////////////////////////////////////////////////////////////////// Getters

	glm::vec3 PositionalLight::getPosition() const {
		return position->getAbsolutePosition();
	}

	PositionalLight::LightAttenuation PositionalLight::getAttenuation() const {
		return attenuation;
	}

	/////////////////////////////////////////////////////////////////// Shaders
	void PositionalLight::updateShaders(ShaderProgram* shader) {
		glm::vec3 eyeSpaceAbsolutePos = camera->getViewMatrix() * glm::vec4(position->getAbsolutePosition(), 1.0);
		shader->setUniformBool (LIGHT_IS_ENABLED,	enabled);
		shader->setUniformVec3f(LIGHT_POSITION,		glm::value_ptr(eyeSpaceAbsolutePos));
		shader->setUniformVec3f(LIGHT_AMBIENT,		glm::value_ptr(ambientColor));
		shader->setUniformVec3f(LIGHT_DIFFUSE,		glm::value_ptr(diffuseColor));
		shader->setUniformVec3f(LIGHT_SPECULAR,		glm::value_ptr(specularColor));
		shader->setUniformFloat(LIGHT_ATTENUATION_CONSTANT,  attenuation.constant);
		shader->setUniformFloat(LIGHT_ATTENUATION_LINEAR,	 attenuation.linear);
		shader->setUniformFloat(LIGHT_ATTENUATION_QUADRATIC, attenuation.quadratic);
	}

}