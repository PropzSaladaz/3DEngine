#include <mgl/scene/mglPositionalLight.hpp>
#include <utils/Logger.hpp>

namespace mgl {
	const GLfloat PositionalLight::DEFAULT_ATTENUATION = 200.0f;

	/////////////////////////////////////////////////////////////////// Constructor
	PositionalLight::PositionalLight() : Light() {
		setColor(COLOR_WHITE);
		setAttenuation(DEFAULT_ATTENUATION);
	}
	PositionalLight::PositionalLight(const std::shared_ptr<SceneObject> position) 
		: PositionalLight() {
		setPosition(position);
	}

	PositionalLight::PositionalLight(const math::vec3& position) 
		: PositionalLight() {
		setPosition(position);
	}

	PositionalLight::PositionalLight(const math::vec3& position, 
		const math::vec3& color) : PositionalLight(position) {
		setColor(color);
	}

	PositionalLight::PositionalLight(const std::shared_ptr<SceneObject> position, 
		const math::vec3& color) : PositionalLight(position) {
		setColor(color);
	}

	/////////////////////////////////////////////////////////////////// Setters

	void PositionalLight::setPosition(const math::vec3& position) {
		std::shared_ptr<SceneObject> lightPos = std::make_shared<SceneObject>();
		lightPos->setPosition(position);
		this->position = lightPos;
	}

	void PositionalLight::setPosition(const std::shared_ptr<SceneObject> position) {
		this->position = position;
	}

	void PositionalLight::setAttenuation(ui32 distance) {
		ui32 lower = attenuationDistances[0];

		if (distance < lower) {
			MGL_ERROR("Attenuation distance must be >= " + std::to_string(lower));
			exit(EXIT_FAILURE);
		};

		for (ui32 attDist : attenuationDistances) {
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

	math::vec3 PositionalLight::getPosition() const {
		return position->getAbsolutePosition();
	}

	PositionalLight::LightAttenuation PositionalLight::getAttenuation() const {
		return attenuation;
	}

	/////////////////////////////////////////////////////////////////// Shaders
	void PositionalLight::updateShaders(ShaderProgram& shader) {
		math::vec3 WorldSpaceAbsolutePos = position->getAbsolutePosition();
		shader.setUniform(LIGHT_IS_ENABLED,	enabled);
		shader.setUniform(LIGHT_POSITION,		WorldSpaceAbsolutePos);
		shader.setUniform(LIGHT_AMBIENT,		ambientColor);
		shader.setUniform(LIGHT_DIFFUSE,		diffuseColor);
		shader.setUniform(LIGHT_SPECULAR,		specularColor);
		shader.setUniform(LIGHT_ATTENUATION_CONSTANT,  attenuation.constant);
		shader.setUniform(LIGHT_ATTENUATION_LINEAR,	 attenuation.linear);
		shader.setUniform(LIGHT_ATTENUATION_QUADRATIC, attenuation.quadratic);
	}

}