#include <mgl/models/meshes/mglMeshManager.hpp>

namespace mgl {

MeshManager::MeshManager() {}
MeshManager::~MeshManager() {}

void MeshManager::import(const std::string& name, const std::string& filePath) {
	Mesh* mesh = new Mesh();

	if (itemCallback) {
		itemCallback(mesh);
	}

	mesh->createFromFile(filePath);
	add(name, mesh);
}

void MeshManager::meshConfigCallback(SetManagedItemCallback callback) {
	setManagedItemCallback(callback);
}

}