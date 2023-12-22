#include <mgl/mglShaders.hpp>
#include <utils/file.hpp>
#include <utils/logger.hpp>
#include <string>

namespace mgl {

////////////////////////////////////////////////////////////////// ShaderUpdator

void ShaderUpdator::declareShaderUniforms(ShaderProgram* shaders) {
    util::Logger::LogError("DeclareShaderUniforms is not defined!");
    exit(EXIT_FAILURE);
}

////////////////////////////////////////////////////////////////// ShaderProgram

ShaderProgram::ShaderProgram() : ProgramId(glCreateProgram()) {}

ShaderProgram::~ShaderProgram() {
  glUseProgram(0);
  glDeleteProgram(ProgramId);
}

void ShaderProgram::addShader(const GLenum shader_type,const std::string &filename) {
    const std::string scode = file::readFile(filename);
    if (scode == FILE_DOESNT_EXIST) {
        util::Logger::LogError("File not found: " + filename);
        exit(EXIT_FAILURE);
    }

    const GLchar *code = scode.c_str();
    const GLuint shader_id = glCreateShader(shader_type);
    glShaderSource(shader_id, 1, &code, 0);
    glCompileShader(shader_id);
    checkCompilation(shader_id, filename);
    glAttachShader(ProgramId, shader_id);

    Shaders[shader_type] = {shader_id};
}

void ShaderProgram::addAttribute(const std::string &name, const GLuint index) {
  glBindAttribLocation(ProgramId, index, name.c_str());
  Attributes[name] = {index};
}

bool ShaderProgram::isAttribute(const std::string &name) {
  return Attributes.find(name) != Attributes.end();
}

void ShaderProgram::addUniform(const std::string &name) {
  Uniforms[name] = {-1};
}

bool ShaderProgram::isUniform(const std::string &name) {
  return Uniforms.find(name) != Uniforms.end();
}

void ShaderProgram::addUniformBlock(const std::string &name,const GLuint binding_point) {
  Ubos[name] = {0, binding_point};
}

bool ShaderProgram::isUniformBlock(const std::string &name) {
  return Ubos.find(name) != Ubos.end();
}

void ShaderProgram::create() {
    glLinkProgram(ProgramId);
    checkLinkage();
    for (auto &i : Shaders) {
        glDetachShader(ProgramId, i.second);
        glDeleteShader(i.second);
    }

    for (auto &i : Uniforms) {
        i.second.index = glGetUniformLocation(ProgramId, i.first.c_str());
        if (i.second.index < 0)
            util::Logger::LogWarning("Uniform " + i.first + " not found");
    }
    for (auto &i : Ubos) {
        i.second.index = glGetUniformBlockIndex(ProgramId, i.first.c_str());
        if (i.second.index < 0)
            util::Logger::LogWarning("UBO " + i.first + " not found");
        glUniformBlockBinding(ProgramId, i.second.index, i.second.binding_point);
    }
}

void ShaderProgram::bind() { glUseProgram(ProgramId); }

void ShaderProgram::unbind() { glUseProgram(0); }


////////////////////////////////////////////////////////////// Set Uniforms

void ShaderProgram::setUniformBool(const std::string &name, bool value) {
#ifdef DEBUG
    assertUniform(name);
#endif
    glUniform1i(Uniforms[name].index, (int)value);
}

void ShaderProgram::setUniformInt(const std::string &name, int value) {
#ifdef DEBUG
    assertUniform(name);
#endif
    glUniform1i(Uniforms[name].index, value);
}

void ShaderProgram::setUniformFloat(const std::string &name, float value) {
#ifdef DEBUG
    assertUniform(name);
#endif
    glUniform1f(Uniforms[name].index, value);
}

void ShaderProgram::setUniformVec4f(GLint varID, const GLfloat * vec) {
    glUniform4fv(varID, 1, vec);
}

void ShaderProgram::setUniformVec4f(const std::string& name, const GLfloat* vec) {
#ifdef DEBUG
    assertUniform(name);
#endif
    setUniformVec4f(Uniforms[name].index, vec);
}
void ShaderProgram::setUniformVec3f(GLint varID, const GLfloat* vec) {
    glUniform3fv(varID, 1, vec);
}

void ShaderProgram::setUniformVec3f(const std::string& name, const GLfloat* vec) {
#ifdef DEBUG
    assertUniform(name);
#endif
    setUniformVec3f(Uniforms[name].index, vec);
}

void ShaderProgram::setUniformMatrix(GLint varID, const GLfloat* matrix) {
    glUniformMatrix4fv(varID, 1, GL_FALSE, matrix);
}

void ShaderProgram::setUniformMatrix(const std::string &name, const GLfloat * matrix) {
#ifdef DEBUG
    assertUniform(name);
#endif
    setUniformMatrix(Uniforms[name].index, matrix);
}



#ifdef DEBUG
void ShaderProgram::assertUniform(const std::string &name) {
    if (!isUniform(name)) {
        util::Logger::LogError("Uniform with name " + name + 
            " doesn't exist in the shader program " + std::to_string(ProgramId));
        exit(EXIT_FAILURE);
    }
}
#endif

// PRIVATE
const GLuint ShaderProgram::checkCompilation(const GLuint shader_id, const std::string &filename) {
  GLint compiled;
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled);
  if (compiled == GL_FALSE) {
    GLint length;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
    GLchar *const log = new char[length];
    glGetShaderInfoLog(shader_id, length, &length, log);
    util::Logger::LogError("Shader compilation check failed:  " + filename + "\n" + log);
    delete[] log;
    exit(EXIT_FAILURE);
  }
  return compiled;
}

void ShaderProgram::checkLinkage() {
  GLint linked;
  glGetProgramiv(ProgramId, GL_LINK_STATUS, &linked);
  if (linked == GL_FALSE) {
    GLint length;
    glGetProgramiv(ProgramId, GL_INFO_LOG_LENGTH, &length);
    GLchar *const log = new char[length];
    glGetProgramInfoLog(ProgramId, length, &length, log);
    util::Logger::LogError(std::string("Shader link check failed:  ") + log);
    delete[] log;
    exit(EXIT_FAILURE);
  }
}

} // namespace mgl