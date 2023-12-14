#ifndef MGL_SHADER_MANAGER_HPP
#define MGL_SHADER_MANAGER_HPP


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <mgl/scene/mglMesh.hpp>
#include <mgl/mglShaders.hpp>
#include <mgl/mglManager.hpp>
#include <iostream>
#include <string>
#include <map>
#include <functional>

namespace mgl {


	class ShaderManager : public Manager<ShaderProgram>{
	public:
		void add(const std::string& name, ShaderProgram* program) override;
	};
}

#endif