#include <mgl/scene/mglSpotLight.hpp>
#include <mgl/scene/mglLight.hpp>
#include <utils/Logger.hpp>

namespace mgl {
	/////////////////////////////////////////////////////////////////////////////// Constant definition

	const GLfloat SpotLight::DEFAULT_INNER_COS_CUTOFF = math::cos(math::radians(10.0f));
	const GLfloat SpotLight::DEFAULT_OUTER_COS_CUTOFF = math::cos(math::radians(12.0f));
	const GLfloat SpotLight::DEFAULT_EPSILON = (DEFAULT_OUTER_COS_CUTOFF - DEFAULT_INNER_COS_CUTOFF);


	/////////////////////////////////////////////////////////////////////////////// Constructors
	SpotLight::SpotLight() : PositionalLight() {}
	SpotLight::SpotLight(const math::vec3 &position) : 
		PositionalLight(position) {}
	SpotLight::SpotLight(const SceneObject* position) : PositionalLight(position) {}
	SpotLight::SpotLight(const math::vec3 &position, const math::vec3& color) : 
		PositionalLight(position, color) {}
	SpotLight::SpotLight(const SceneObject* position, const math::vec3 &color) 
		: PositionalLight(position, color) {}
	SpotLight::SpotLight(const math::vec3 &position, const math::vec3 &color, const math::vec3 &target) :
		PositionalLight(position, color) {
		setTarget(target);
	}
	SpotLight::SpotLight(const math::vec3 &position, const math::vec3 &color, const SceneObject* target) :
		PositionalLight(position, color) {
		setTarget(target);
	}
	SpotLight::SpotLight(const SceneObject* position, const math::vec3& color, const math::vec3& target) :
		PositionalLight(position, color) {
		setTarget(target);
	}
	SpotLight::SpotLight(const SceneObject* position, const math::vec3& color, const SceneObject* target) :
		PositionalLight(position, color) {
		setTarget(target);
	}

	///////////////////////////////////////////////////// Setters

	void SpotLight::setTarget(const math::vec3 &target) {
		mgl::SceneObject* sceneObj = new SceneObject();
		sceneObj->setPosition(target);
		this->target = sceneObj;
	}
	void SpotLight::setTarget(const SceneObject* direction) {
		this->target = direction;
	}

	void SpotLight::setInnerCutoffAngle(GLfloat angleDegrees) {
		if (angleDegrees >= 90.0f || angleDegrees <= 0.0f) {
			MGL_ERROR("Inner angle must be in [0, 90]");
			exit(EXIT_FAILURE);
		}
		spotInnerCosCutoff = math::cos(math::radians(angleDegrees));
	}
	void SpotLight::setOuterCutoffAngle(GLfloat angleDegrees) {
		if (angleDegrees >= 90.0f || angleDegrees <= 0.0f) {
			MGL_ERROR("Outer angle must be in [0, 90] - Angle received: " 
				+ std::to_string(angleDegrees));
			exit(EXIT_FAILURE);
		}
		
		GLfloat cosCutoff = math::cos(math::radians(angleDegrees));
		
		if (cosCutoff > spotInnerCosCutoff) {
			MGL_ERROR("Outer angle must be larger than inner angle.");
			exit(EXIT_FAILURE);
		}

		spotOuterCosCutoff = cosCutoff;
		epsilon = spotInnerCosCutoff - spotOuterCosCutoff;
	}

	/////////////////////////////////////////////////////// Getters

	math::vec3 SpotLight::getDirection() {
		return target->getAbsolutePosition() - position->getAbsolutePosition();
	}

	/////////////////////////////////////////////////////// Update Shaders

	void SpotLight::updateShaders(ShaderProgram* shader) {
		PositionalLight::updateShaders(shader);
		shader->setUniform(LIGHT_LIGHT_TYPE, SPOT_LIGHT);
		shader->setUniform(LIGHT_SPOT_OUTER_COS_CUTOFF, spotOuterCosCutoff);
		shader->setUniform(LIGHT_EPSILON,	  epsilon);
		shader->setUniform(LIGHT_DIRECTION, math::vec4(getDirection(), 0.0));
	}

}