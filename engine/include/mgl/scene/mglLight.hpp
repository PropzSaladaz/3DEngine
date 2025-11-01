#ifndef MGL_LIGHT_HPP
#define MGL_LIGHT_HPP

#include <cstring>
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
	static inline char* LIGHT_UNIFORM(ui32 light_nr, const char* propertyName) {
		std::string result = "Lights[" + std::to_string(light_nr) + "]." + propertyName;
		return strdup(result.c_str());
	}


	enum LightTtypes { DIRECTIONAL_LIGHT, POINT_LIGHT, SPOT_LIGHT };

	static const ui32 MAX_NR_LIGHTS;
	static const i32 LIGHT_TYPE;

	static ui32 LIGHT_NR;

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

	void setColor(const math::vec3& color);
	void setAmbient(const math::vec3& ambient);
	void setDiffuse(const math::vec3& diffuse);
	void setSpecular(const math::vec3& specular);

	math::vec3 getAmbient() const;
	math::vec3 getDiffuse() const;
	math::vec3 getSpecular() const;

protected:
	bool enabled;
	math::vec4 ambientColor;
	math::vec4 diffuseColor;
	math::vec4 specularColor;
	const Camera* camera;
};

}
#endif