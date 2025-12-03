#include <mgl/shaders/ShaderUpdator.hpp>
#include <mgl/shaders/ShaderBuilder.hpp>
#include <utils/Logger.hpp>
#include <stdexcept>

namespace mgl {

void ShaderUpdator::declareShaderUniforms(ShaderBuilder& shaders) {
    MGL_ERROR("DeclareShaderUniforms is not defined for this ShaderUpdator.");
    throw std::logic_error("ShaderUpdator::declareShaderUniforms must be overridden");
}

} // namespace mgl
