#ifndef MGL_SHADERS_HPP
#define MGL_SHADERS_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <map>

namespace mgl {

class ShaderProgram;

class ShaderProgram {
public:
    GLuint ProgramId;

    struct ShaderInfo {
        GLuint index;
    };
    std::map<GLenum, GLuint> Shaders;

    struct AttributeInfo {
        GLuint index;
    };
    std::map<std::string, AttributeInfo> Attributes;

    struct UniformInfo {
        GLint index;
    };
    std::map<std::string, UniformInfo> Uniforms;

    struct UboInfo {
        GLuint index;
        GLuint binding_point;
    };
    std::map<std::string, UboInfo> Ubos;

    ShaderProgram();
    ~ShaderProgram();
    void addShader(const GLenum shader_type, const std::string &filename);
    void addAttribute(const std::string &name, const GLuint index);
    bool isAttribute(const std::string &name);
    void addUniform(const std::string &name);
    bool isUniform(const std::string &name);
    void addUniformBlock(const std::string &name, const GLuint binding_point);
    bool isUniformBlock(const std::string &name);
    void create();
    void bind();
    void unbind();
    void setUniformBool(const std::string &name, bool value);
    void setUniformInt(const std::string &name, int value);
    void setUniformFloat(const std::string &name, float value);
    void setUniformVec3f(GLint varID, const GLfloat* vec);
    void setUniformVec3f(const std::string& name, const GLfloat* vec);
    void setUniformVec4f(GLint varID, const GLfloat * vec);
    void setUniformVec4f(const std::string& name, const GLfloat* vec);
    void setUniformMatrix(const std::string &name, const GLfloat * matrix);
    void setUniformMatrix(GLint varID, const GLfloat * matrix);

 private:
    const GLuint checkCompilation(const GLuint shader_id,const std::string &filename);
    void checkLinkage();

#ifdef DEBUG
    void assertUniform(const std::string& name);
    void assertAttribute(const std::string& name);
#endif
};


} // namespace mgl

#endif /* MGL_SHADERS_HPP */