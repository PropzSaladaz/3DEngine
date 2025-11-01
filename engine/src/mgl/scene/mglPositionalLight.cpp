#include <mgl/scene/mglPositionalLight.hpp>
#include <utils/Logger.hpp>

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

	PositionalLight::PositionalLight(const math::vec3& position) 
		: PositionalLight() {
		setPosition(position);
	}

	PositionalLight::PositionalLight(const math::vec3& position, 
		const math::vec3& color) : PositionalLight(position) {
		setColor(color);
	}

	PositionalLight::PositionalLight(const SceneObject* position, 
		const math::vec3& color) : PositionalLight(position) {
		setColor(color);
	}

	/////////////////////////////////////////////////////////////////// Setters

	void PositionalLight::setPosition(const math::vec3& position) {
		mgl::SceneObject* lightPos = new SceneObject();
		lightPos->setPosition(position);
		this->position = lightPos;
	}

	void PositionalLight::setPosition(const SceneObject* position) {
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
	void PositionalLight::updateShaders(ShaderProgram* shader) {
		math::vec3 WorldSpaceAbsolutePos = position->getAbsolutePosition();
		shader->setUniformBool (LIGHT_IS_ENABLED,	enabled);
		shader->setUniformVec3f(LIGHT_POSITION,		WorldSpaceAbsolutePos.data());
		shader->setUniformVec4f(LIGHT_AMBIENT,		ambientColor.data());
		shader->setUniformVec4f(LIGHT_DIFFUSE,		diffuseColor.data());
		shader->setUniformVec4f(LIGHT_SPECULAR,		specularColor.data());
		shader->setUniformFloat(LIGHT_ATTENUATION_CONSTANT,  attenuation.constant);
		shader->setUniformFloat(LIGHT_ATTENUATION_LINEAR,	 attenuation.linear);
		shader->setUniformFloat(LIGHT_ATTENUATION_QUADRATIC, attenuation.quadratic);
	}

}