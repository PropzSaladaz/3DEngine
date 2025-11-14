#pragma once


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <mgl/models/meshes/mglMesh.hpp>
#include <mgl/mglShaders.hpp>
#include <mgl/mglManager.hpp>
#include <mgl/shaders/ShaderProgram.hpp>
#include <iostream>
#include <string>
#include <map>
#include <functional>

namespace mgl {


	class ShaderManager : public Manager<ShaderProgram>{
	public:
		void add(const std::string& name, std::shared_ptr<ShaderProgram> program) override;
	};
}