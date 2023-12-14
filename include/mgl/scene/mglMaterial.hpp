#ifndef MGL_MATERIAL_HPP
#define MGL_MATERIAL_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <vector>

#include <mgl/mglShaders.hpp>

namespace mgl {

class Material;

const glm::vec3 COLOR_WHITE	(1, 1, 1);
const glm::vec3 COLOR_RED	(1, 0, 0);
const glm::vec3 COLOR_GREEN	(0, 1, 0);
const glm::vec3 COLOR_BLUE	(0, 0, 1);
/////////////////////////////////////////////////////////////////////////// Material
class Material { // TODO make it abstract
public:
	ShaderProgram* Shaders;
	Material(ShaderProgram* shaders);
	void bind();
	void unbind();
	Material* setColor(const glm::vec3 &color);

private:
	glm::vec3 color;
};

////////////////////////////////////////////////////////////////////////////////
}  // namespace mgl

#endif /* MGL_MATERIAL_HPP  */
