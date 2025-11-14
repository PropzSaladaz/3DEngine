#include <mgl/mglShaderManager.hpp>

namespace mgl {

void ShaderManager::add(const std::string& name, std::shared_ptr<ShaderProgram> program) {
	Manager::add(name, program);
}

}