#ifndef MGL_LIGHT_HPP
#define MGL_LIGHT_HPP

#include <glm/glm.hpp>
#include <mgl/mglShaders.hpp>
#include <mgl/camera/mglCamera.hpp>

namespace mgl {

class Light;
class PointLight;

class Light : public ShaderUpdator {
public:
	// shader must have a struct, with vec3 attributes
	static const char LIGHT_POSITION[];
	static const char LIGHT_AMBIENT[];
	static const char LIGHT_DIFFUSE[];
	static const char LIGHT_SPECULAR[];

	static void declareShaderUniforms(ShaderProgram* shaders);

	Light();
	~Light();
	void enable();
	void disable();
	bool isEnabled();

	void assignCamera(const Camera* camera);

	void virtual setColor(const glm::vec3& color) = 0;
protected:
	bool enabled;
	const Camera* camera;
};

}
#endif