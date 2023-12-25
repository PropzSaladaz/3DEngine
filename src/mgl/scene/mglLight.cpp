#include <mgl/scene/mglLight.hpp>
#include <mgl/camera/mglCamera.hpp>
#include <utils/logger.hpp>
#include <cstring>
namespace mgl {

	const GLuint Light::MAX_NR_LIGHTS = 10;
	const GLint Light::LIGHT_TYPE = -1;

	GLuint Light::LIGHT_NR = 0; // current light number

	// fixed property names
	const char Light::LIGHT_IS_ENABLED_PROP[]	= "isEnabled";
	const char Light::LIGHT_LIGHT_TYPE_PROP[]	= "lightType";
	const char Light::LIGHT_POSITION_PROP[]		= "position";
	const char Light::LIGHT_DIRECTION_PROP[]	= "direction";
	const char Light::LIGHT_AMBIENT_PROP[]		= "ambient";
	const char Light::LIGHT_DIFFUSE_PROP[]		= "diffuse";
	const char Light::LIGHT_SPECULAR_PROP[]		= "specular";
	const char Light::LIGHT_EPSILON_PROP[]		= "epsilon";
	const char Light::LIGHT_SPOT_OUTER_COS_CUTOFF_PROP[] = "spotOuterCosCutoff";
	const char Light::LIGHT_ATTENUATION_CONSTANT_PROP[]  = "constantAttenuation";
	const char Light::LIGHT_ATTENUATION_LINEAR_PROP[]    = "linearAttenuation";
	const char Light::LIGHT_ATTENUATION_QUADRATIC_PROP[] = "quadraticAttenuation";

	void Light::declareShaderUniforms(ShaderProgram* shaders) {
		// declare all light unifoms
		for (int i = 0; i < MAX_NR_LIGHTS; i++) {
			shaders->addUniform(LIGHT_UNIFORM(i, LIGHT_IS_ENABLED_PROP));
			shaders->addUniform(LIGHT_UNIFORM(i, LIGHT_LIGHT_TYPE_PROP));
			shaders->addUniform(LIGHT_UNIFORM(i, LIGHT_POSITION_PROP));
			shaders->addUniform(LIGHT_UNIFORM(i, LIGHT_DIRECTION_PROP));
			shaders->addUniform(LIGHT_UNIFORM(i, LIGHT_AMBIENT_PROP));
			shaders->addUniform(LIGHT_UNIFORM(i, LIGHT_DIFFUSE_PROP));
			shaders->addUniform(LIGHT_UNIFORM(i, LIGHT_SPECULAR_PROP));
			shaders->addUniform(LIGHT_UNIFORM(i, LIGHT_SPOT_OUTER_COS_CUTOFF_PROP));
			shaders->addUniform(LIGHT_UNIFORM(i, LIGHT_EPSILON_PROP));
			shaders->addUniform(LIGHT_UNIFORM(i, LIGHT_ATTENUATION_CONSTANT_PROP));
			shaders->addUniform(LIGHT_UNIFORM(i, LIGHT_ATTENUATION_LINEAR_PROP));
			shaders->addUniform(LIGHT_UNIFORM(i, LIGHT_ATTENUATION_QUADRATIC_PROP));
		}
	}

	Light::Light() {
		enabled = true;
		if (LIGHT_NR > MAX_NR_LIGHTS) {
			util::Logger::LogError("Max number of lights reached");
			exit(EXIT_FAILURE);
		}
		// set uniform properties with the light number index
		LIGHT_IS_ENABLED	= LIGHT_UNIFORM(LIGHT_NR, LIGHT_IS_ENABLED_PROP);
		LIGHT_LIGHT_TYPE	= LIGHT_UNIFORM(LIGHT_NR, LIGHT_LIGHT_TYPE_PROP);
		LIGHT_POSITION		= LIGHT_UNIFORM(LIGHT_NR, LIGHT_POSITION_PROP);
		LIGHT_DIRECTION		= LIGHT_UNIFORM(LIGHT_NR, LIGHT_DIRECTION_PROP);
		LIGHT_AMBIENT		= LIGHT_UNIFORM(LIGHT_NR, LIGHT_AMBIENT_PROP);
		LIGHT_DIFFUSE		= LIGHT_UNIFORM(LIGHT_NR, LIGHT_DIFFUSE_PROP);
		LIGHT_SPECULAR		= LIGHT_UNIFORM(LIGHT_NR, LIGHT_SPECULAR_PROP);
		LIGHT_SPOT_OUTER_COS_CUTOFF = LIGHT_UNIFORM(LIGHT_NR, LIGHT_SPOT_OUTER_COS_CUTOFF_PROP);
		LIGHT_EPSILON				= LIGHT_UNIFORM(LIGHT_NR, LIGHT_EPSILON_PROP);
		LIGHT_ATTENUATION_CONSTANT	= LIGHT_UNIFORM(LIGHT_NR, LIGHT_ATTENUATION_CONSTANT_PROP);
		LIGHT_ATTENUATION_LINEAR	= LIGHT_UNIFORM(LIGHT_NR, LIGHT_ATTENUATION_LINEAR_PROP);
		LIGHT_ATTENUATION_QUADRATIC = LIGHT_UNIFORM(LIGHT_NR, LIGHT_ATTENUATION_QUADRATIC_PROP);

		setAmbient(glm::vec3(1.0f));
		setDiffuse(glm::vec3(1.0f));
		setSpecular(glm::vec3(1.0f));

		LIGHT_NR++;
	}
	Light::~Light() {}
	void Light::enable() {
		enabled = true;
	}
	void Light::disable() {
		enabled = false;
	}
	bool Light::isEnabled() {
		return enabled;
	}

	void Light::assignCamera(const Camera* camera) {
		this->camera = camera;
	}

	///////////////////////////////////////////////// Setters
	void Light::setColor(const glm::vec3& color) {
		setAmbient(color);
		setDiffuse(color);
		setSpecular(color);
	}
	void Light::setAmbient(const glm::vec3& ambient) {
		ambientColor = glm::vec4(ambient, 1.0f);
	}
	void Light::setDiffuse(const glm::vec3& diffuse) {
		diffuseColor = glm::vec4(diffuse, 1.0f);
	}
	void Light::setSpecular(const glm::vec3& specular) {
		specularColor = glm::vec4(specular, 1.0f);
	}

	////////////////////////////////////////////////// Getters
	glm::vec3 Light::getAmbient() const {
		return ambientColor;
	}
	glm::vec3 Light::getDiffuse() const {
		return diffuseColor;
	}
	glm::vec3 Light::getSpecular() const {
		return specularColor;
	}
}