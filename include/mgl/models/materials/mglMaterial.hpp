#ifndef MGL_MATERIAL_HPP
#define MGL_MATERIAL_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <string>

#include <mgl/mglShaders.hpp>
#include <mgl/models/textures/mglTexture.hpp>

namespace mgl {

const glm::vec3 COLOR_WHITE	(1, 1, 1);
const glm::vec3 COLOR_RED	(1, 0, 0);
const glm::vec3 COLOR_GREEN	(0, 1, 0);
const glm::vec3 COLOR_BLUE	(0, 0, 1);

/////////////////////////////////////////////////////////////////////////// Material
class Material {
public:
	static inline const char MATERIAL_LIGHT_COLOR[] = "lightColor";

	ShaderProgram* Shaders;

	Material(ShaderProgram* shaders);
	void bind();
	void unbind();
	void setTexture(TextureInfo* texture);
	void setTextureSpecular(TextureInfo* texture);
	virtual Material* setColor(const glm::vec3 &color) = 0;

private:
	static GLuint id;
	std::string shader_id;
	TextureInfo* texture = nullptr;
	TextureInfo* textureSpecular = nullptr;

	virtual void setMaterialUniforms(ShaderProgram* shaders) = 0;
	virtual std::string getShaderPrefix();
};

////////////////////////////////////////////////////////////////////////////////
}  // namespace mgl

#endif /* MGL_MATERIAL_HPP  */
