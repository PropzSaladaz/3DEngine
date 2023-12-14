#ifndef MGL_DIRECTIONAL_LIGHT_HPP
#define MGL_DIRECTIONAL_LIGHT_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <vector>

#include <mgl/mglShaders.hpp>

namespace mgl {

	///////////////////////////////////////////////////////////////// Light
	class Light {
	public:
		Light();

	private:
		GLuint VAO;
	};

	////////////////////////////////////////////////////////////////////////////////
}  // namespace mgl

#endif /* MGL_MATERIAL_HPP  */
