#include <mgl/shaders/ShaderProgram.hpp>
#include <utils/file.hpp>
#include <utils/Logger.hpp>
#include <string>

namespace mgl {

////////////////////////////////////////////////////////////////// ShaderUpdator

// void ShaderUpdator::declareShaderUniforms(ShaderProgram* shaders) {
//     MGL_ERROR("DeclareShaderUniforms is not defined!");
//     exit(EXIT_FAILURE);
// }

////////////////////////////////////////////////////////////////// ShaderProgram

ShaderProgram::ShaderProgram() : 
    // glCreateProgram creates a program & returns the ID reference to the newly created 
    // program object - this is will be used when attaching new shaders to this program
    ProgramId(glCreateProgram()) 
{}

ShaderProgram::~ShaderProgram() {
  glUseProgram(0);
  glDeleteProgram(ProgramId);
}

bool ShaderProgram::isAttribute(const std::string &name) {
  return Attributes.find(name) != Attributes.end();
}

bool ShaderProgram::isUniform(const std::string &name) {
  return Uniforms.find(name) != Uniforms.end();
}

bool ShaderProgram::isUniformBlock(const std::string &name) {
  return Ubos.find(name) != Ubos.end();
}

void ShaderProgram::bind() { glUseProgram(ProgramId); }

void ShaderProgram::unbind() { glUseProgram(0); }


////////////////////////////////////////////////////////////// Set Uniforms

// PRIVATE

void ShaderProgram::setUniform(i32 loc, i32 v)              { glUniform1i(loc, v); }
void ShaderProgram::setUniform(i32 loc, bool v)             { glUniform1i(loc, v ? 1 : 0); }
void ShaderProgram::setUniform(i32 loc, f32 v)              { glUniform1f(loc, v); }
void ShaderProgram::setUniform(i32 loc, u32 v)              { glUniform1ui(loc, v); }

void ShaderProgram::setUniform(i32 loc, const math::vec3& v) { glUniform3fv(loc, 1, v.data()); }
void ShaderProgram::setUniform(i32 loc, const math::vec4& v) { glUniform4fv(loc, 1, v.data()); }
void ShaderProgram::setUniform(i32 loc, const math::mat4& m) { glUniformMatrix4fv(loc, 1, GL_FALSE, m.data()); }


void ShaderProgram::assertUniform(const std::string &name) {
    if (!isUniform(name)) {
        MGL_ERROR("Uniform with name " + name + 
            " doesn't exist in the shader program " + std::to_string(ProgramId));
        exit(EXIT_FAILURE);
    }
}

} // namespace mgl