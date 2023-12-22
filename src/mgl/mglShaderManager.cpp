#include <mgl/mglShaderManager.hpp>

namespace mgl {

void ShaderManager::add(const std::string& name, ShaderProgram* program) {
	if (itemCallback) {
		itemCallback(program);
	}

	program->create();
	Manager::add(name, program);
}

void ShaderManager::beforeBuild(SetManagedItemCallback callback) {
	itemCallback = callback;
}

}