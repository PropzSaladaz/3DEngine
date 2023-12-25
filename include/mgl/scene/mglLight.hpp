#ifndef MGL_LIGHT_HPP
#define MGL_LIGHT_HPP

#include <glm/glm.hpp>
#include <mgl/mglShaders.hpp>
#include <mgl/camera/mglCamera.hpp>

namespace mgl {

class Light;
class PointLight;

/*
	Represents an abstract generic light with all the parameters
	needed at the shader
*/
class Light : public ShaderUpdator {
public:
	static inline char* LIGHT_UNIFORM(GLuint light_nr, const char* propertyName) {
		std::string result = "Lights[" + std::to_string(light_nr) + "]." + propertyName;
		return _strdup(result.c_str());
	}


	enum LightTtypes { DIRECTIONAL_LIGHT, POINT_LIGHT, SPOT_LIGHT };

	static const GLuint MAX_NR_LIGHTS;
	static const GLint LIGHT_TYPE;

	static GLuint LIGHT_NR;

	// property name only
	static const char LIGHT_IS_ENABLED_PROP[];
	static const char LIGHT_LIGHT_TYPE_PROP[];
	static const char LIGHT_POSITION_PROP[];
	static const char LIGHT_DIRECTION_PROP[];
	static const char LIGHT_AMBIENT_PROP[];
	static const char LIGHT_DIFFUSE_PROP[];
	static const char LIGHT_SPECULAR_PROP[];
	static const char LIGHT_SPOT_OUTER_COS_CUTOFF_PROP[];
	static const char LIGHT_EPSILON_PROP[];
	static const char LIGHT_ATTENUATION_CONSTANT_PROP[];
	static const char LIGHT_ATTENUATION_LINEAR_PROP[];
	static const char LIGHT_ATTENUATION_QUADRATIC_PROP[];

	// full property name "Lights[N].propName"
	const char *LIGHT_IS_ENABLED;
	const char *LIGHT_LIGHT_TYPE;
	const char *LIGHT_POSITION;
	const char *LIGHT_DIRECTION;
	const char *LIGHT_AMBIENT;
	const char *LIGHT_DIFFUSE;
	const char *LIGHT_SPECULAR;
	const char *LIGHT_SPOT_OUTER_COS_CUTOFF;
	const char *LIGHT_EPSILON;
	const char *LIGHT_ATTENUATION_CONSTANT;
	const char *LIGHT_ATTENUATION_LINEAR;
	const char *LIGHT_ATTENUATION_QUADRATIC;

	static void declareShaderUniforms(ShaderProgram* shaders);

	Light();
	~Light();
	void enable();
	void disable();
	bool isEnabled();

	void assignCamera(const Camera* camera);

	void setColor(const glm::vec3& color);
	void setAmbient(const glm::vec3& ambient);
	void setDiffuse(const glm::vec3& diffuse);
	void setSpecular(const glm::vec3& specular);

	glm::vec3 getAmbient() const;
	glm::vec3 getDiffuse() const;
	glm::vec3 getSpecular() const;

protected:
	bool enabled;
	glm::vec4 ambientColor;
	glm::vec4 diffuseColor;
	glm::vec4 specularColor;
	const Camera* camera;
};

}
#endif