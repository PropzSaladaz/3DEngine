#ifndef MGL_MESH_MANAGER_HPP
#define MGL_MESH_MANAGER_HPP


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <mgl/scene/mglMesh.hpp>
#include <mgl/mglManager.hpp>
#include <iostream>
#include <string>
#include <map>
#include <functional>

namespace mgl {

	class MeshManager;

	class MeshManager : public Manager<Mesh> {
	public:
		MeshManager();
		~MeshManager();
		void import(const std::string& name, const std::string& filePath);
	};
}

#endif // !MGL_MESH_MANAGER_HPP
