#include <mgl/shaders/ShaderBuilder.hpp>
#include <mgl/shaders/ShaderProgram.hpp>
#include <utils/file.hpp>
#include <utils/Logger.hpp>
#include <string>
#include "types.hpp"

namespace mgl {

////////////////////////////////////////////////////////////////// ShaderUpdator



////////////////////////////////////////////////////////////////// ShaderBuilder

void ShaderBuilder::addShader(const GLenum shader_type,const std::string &filename) {
    // update the link between the shader type and the coresponding shader id
    // in current program. We can only have 1 shader for each different shader type
    Shaders[shader_type] = { filename };
}

void ShaderBuilder::addAttribute(const std::string &name, const i32 index) {
  Attributes[name] = {index};
}


ui32 ShaderBuilder::buildShader(const GLenum shader_type, const std::string &filename) {
    // try reading shader from file
    const std::string scode = file::readFile(filename);
    if (scode == FILE_DOESNT_EXIST) {
        MGL_ERROR("File not found: " + filename);
        exit(EXIT_FAILURE);
    }
    const GLchar *code = scode.c_str();
    
    // specify type of shader we want to create
    const GLuint shader_id = glCreateShader(shader_type);

    // set the shader code for the created shader with id = shader_id
    glShaderSource(shader_id, 1, &code, 0);

    // compile the shader
    glCompileShader(shader_id);

    checkCompilation(shader_id, filename);
    return shader_id;
}

ShaderBuilder::ShaderBuilder() {}
ShaderBuilder::~ShaderBuilder() {}



ShaderProgram ShaderBuilder::build() {
    ui32 programId = glCreateProgram();
    assert(glIsProgram(programId));

    // Compile + attach all sahders
    std::vector<ui32> shaderIds;
    for (const auto& [shaderType, shaderDecl] : Shaders) {
        ui32 shaderId = buildShader(shaderType, shaderDecl.fileName);
        glAttachShader(programId, shaderId);
        shaderIds.push_back(shaderId);
    }

    // Bind all attributes
    for (const auto& [name, attrDecl] : Attributes) {
        glBindAttribLocation(programId, attrDecl.index, name.c_str());
    }

    // Link + check linkage
    glLinkProgram(programId);
    checkLinkage(programId);

    // individual shaders are not longer needed - they are already linked
    // to the shader program - we can delete them
    for (auto &id : shaderIds) {
        glDetachShader(programId, id);
        glDeleteShader(id);
    }

    // Bind all uniforms and UBOs
    for (auto& [name, u] : Uniforms) {
        u.index = glGetUniformLocation(programId, name.c_str());
        if (u.index < 0) {
            MGL_WARN("Uniform '" + name + "' not found or optimized out");
        }
    }

    for (auto& [name, b] : Ubos) {
        b.index = glGetUniformBlockIndex(programId, name.c_str());
        if (b.index == GL_INVALID_INDEX) {
            MGL_WARN("UBO block '" + name + "' not found");
        } else {
            glUniformBlockBinding(programId, b.index, b.binding_point);
        }
    }

    // Attribute validation (optional but useful)
    // Reflect actual attribute locations and compare to requested
    {
        GLint numAttribs = 0;
        glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTES, &numAttribs);
        // Build a name->location map to verify requested indices:
        for (const auto& [name, attrDecl] : Attributes) {
            GLint loc = glGetAttribLocation(programId, name.c_str());
            if (loc < 0) {
                MGL_WARN("Attribute '" + name + "' not active");
            } else if (loc != static_cast<GLint>(attrDecl.index)) {
                MGL_WARN("Attribute '" + name + "' linked at " + std::to_string(loc) +
                         " but requested " + std::to_string(attrDecl.index));
            }
        }
    }

    std::unordered_map<std::string, ShaderProgram::AttributeInfo> shaderAttributes;
    for (const auto& [name, attrDecl] : Attributes) {
        shaderAttributes[name] = { static_cast<i32>(attrDecl.index) };
    }
    std::unordered_map<std::string, ShaderProgram::UniformInfo> shaderUniforms;
    for (const auto& [name, uniformDecl] : Uniforms) {
        shaderUniforms[name] = { uniformDecl.index };
    }
    std::unordered_map<std::string, ShaderProgram::UboInfo> shaderUbos;
    for (const auto& [name, uboDecl] : Ubos) {
        shaderUbos[name] = { uboDecl.index, static_cast<i32>(uboDecl.binding_point) };
    }
    return ShaderProgram(
        programId, 
        shaderAttributes, 
        shaderUniforms, 
        shaderUbos);
}





bool ShaderBuilder::isAttribute(const std::string &name) {
  return Attributes.find(name) != Attributes.end();
}

void ShaderBuilder::addUniform(const std::string &name) {
  Uniforms[name] = {-1};
}

bool ShaderBuilder::isUniform(const std::string &name) {
  return Uniforms.find(name) != Uniforms.end();
}

void ShaderBuilder::addUniformBlock(const std::string &name,const ui32 binding_point) {
  Ubos[name] = {0, binding_point};
}

bool ShaderBuilder::isUniformBlock(const std::string &name) {
  return Ubos.find(name) != Ubos.end();
}

////////////////////////////////////////////////////////////// Set Uniforms

const void ShaderBuilder::checkCompilation(const ui32 shader_id, const std::string &filename) {
  i32 compiled; // stores if shader compiled successfully or not
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled);
  if (compiled == GL_FALSE) {
    i32 length;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
    GLchar *const log = new char[length];
    glGetShaderInfoLog(shader_id, length, &length, log);
    MGL_ERROR("Shader compilation check failed:  " + filename + "\n" + log);
    delete[] log;
    exit(EXIT_FAILURE);
  }
}

void ShaderBuilder::checkLinkage(ui32 programId) {
  i32 linked;
  glGetProgramiv(programId, GL_LINK_STATUS, &linked);
  if (linked == GL_FALSE) {
    i32 length;
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &length);
    GLchar *const log = new char[length];
    glGetProgramInfoLog(programId, length, &length, log);
    MGL_ERROR("Shader link check failed:  " + std::string(log));
    delete[] log;
    exit(EXIT_FAILURE);
  }
}

} // namespace mgl