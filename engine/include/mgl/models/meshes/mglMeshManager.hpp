#ifndef MGL_MESH_MANAGER_HPP
#define MGL_MESH_MANAGER_HPP

#include <mgl/models/meshes/mglMesh.hpp>
#include <mgl/mglManager.hpp>
#include <string>

namespace mgl {

	class MeshManager;

	class MeshManager : public Manager<Mesh> {
	public:
		MeshManager();
		~MeshManager();
		void import(const std::string& name, const std::string& filePath);
		void meshConfigCallback(SetManagedItemCallback callback);
	};
}

#endif // !MGL_MESH_MANAGER_HPP
