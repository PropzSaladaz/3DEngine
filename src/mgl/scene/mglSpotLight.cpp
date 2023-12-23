#include <mgl/scene/mglSpotLight.hpp>
#include <mgl/scene/mglLight.hpp>
#include <utils/logger.hpp>

namespace mgl {

	SpotLight::SpotLight(glm::vec3 position) : 
		PositionalLight(position) {}
	SpotLight::SpotLight(glm::vec3 position, glm::vec3 color) : 
		PositionalLight(position, color) {}
	SpotLight::SpotLight(glm::vec3 position, glm::vec3 color, glm::vec3 direction) :
		PositionalLight(position, color) {
		setDirection(direction);
	}
	SpotLight::SpotLight(glm::vec3 position, glm::vec3 color, SceneObject* direction) : 
		PositionalLight(position, color) {
		setDirection(direction);
	}
	///////////////////////////////////////////////////// Setters
	void SpotLight::setDirection(glm::vec3 direction) {
		mgl::SceneObject* sceneObj = new SceneObject();
		sceneObj->setPosition(direction);
		this->direction = sceneObj;
	}
	void SpotLight::setDirection(SceneObject* direction) {
		this->direction = direction;
	}

	void SpotLight::setInnerCutoffAngle(GLfloat angleDegrees) {
		if (angleDegrees >= 90.0f || angleDegrees <= 0.0f) {
			util::Logger::LogError("Inner angle must be in [0, 90]");
			exit(EXIT_FAILURE);
		}
		spotInnerCosCutoff = glm::cos(glm::radians(angleDegrees));
	}
	void SpotLight::setOuterCutoffAngle(GLfloat angleDegrees) {
		if (angleDegrees >= 90.0f || angleDegrees <= 0.0f) {
			util::Logger::LogError("Outer angle must be in [0, 90] - Angle received: " 
				+ std::to_string(angleDegrees));
			exit(EXIT_FAILURE);
		}
		
		GLfloat cosCutoff = glm::cos(glm::radians(angleDegrees));
		
		if (cosCutoff > spotInnerCosCutoff) {
			util::Logger::LogError("Outer angle must be larger than inner angle.");
			exit(EXIT_FAILURE);
		}

		spotOuterCosCutoff = cosCutoff;
		epsilon = spotInnerCosCutoff - spotOuterCosCutoff;
	}

	/////////////////////////////////////////////////////// Getters
	glm::vec3 SpotLight::getDirection() {
		return direction->getAbsolutePosition();
	}


	void SpotLight::updateShaders(ShaderProgram* shader) {
		PositionalLight::updateShaders(shader);
		shader->setUniformInt  (LIGHT_LIGHT_TYPE, SPOT_LIGHT);
		shader->setUniformFloat(LIGHT_SPOT_OUTER_COS_CUTOFF, spotOuterCosCutoff);
		shader->setUniformFloat(LIGHT_EPSILON,	  epsilon);
		shader->setUniformVec3f(LIGHT_DIRECTION, glm::value_ptr(direction->getAbsolutePosition()));
	}

}