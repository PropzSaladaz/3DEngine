#ifndef MGL_SHADERS_HPP
#define MGL_SHADERS_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <map>

namespace mgl {

    class ShaderProgram;
    class ShaderUpdator;

    class ShaderUpdator {
    public:
        /// <summary>
        /// Called when creating a shader program to declare shader uniform's names
        /// </summary>
        /// <param name="shaders"></param>
        static void declareShaderUniforms(ShaderProgram* shaders);

        /// <summary>
        /// Called at runtime to set the values on the predeclared shader uniform names
        /// </summary>
        /// <param name="shaders"></param>
        virtual void updateShaders(ShaderProgram* shaders) = 0;
    };

    /// <summary>
    /// A shader program object is the final linked version of multiple shaders combined. 
    /// To use any shader we must link them into a shader program object and then activate this shader program.
    /// </summary>
    class ShaderProgram {
    public:

        // Stores the OpenGL's shader program object ID
        GLuint ProgramId;

        struct ShaderInfo {
            GLuint index;
        };

        // Links shader type to the OpenGL's shader ID 
        // E.g:
        //      GL_VERTEX_SHADER -> 0
        //      GL_FRAGMENT_SHADER -> 1, and so on
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

        /// <summary>
        /// Compiles the shader code and attaches it to the shader program.
        /// </summary>
        /// <param name="shader_type"> specifies the type of shader to be added to 
        /// the program - GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, etc</param>
        /// <param name="filename"> file name where the shader is located </param>
        void addShader(const GLenum shader_type, const std::string &filename);
        void addAttribute(const std::string &name, const GLuint index);
        bool isAttribute(const std::string &name);
    
        template <typename T> void addUniforms();
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
        /// <summary>
        /// Checks whether a shader program has been compiled successfully or not.
        /// If didn't compile successfully, exits the program
        /// </summary>
        /// <param name="shader_id"> id of the compiled shader</param>
        /// <param name="filename"> shader's file name </param>
        /// <returns> True if compiled successfully. Flase otherwise</returns>
        const void checkCompilation(const GLuint shader_id,const std::string &filename);

        /// <summary>
        /// Checks whether the shader program was linked correctly. If not, exits the program
        /// </summary>
        void checkLinkage();

#ifdef DEBUG
        void assertUniform(const std::string& name);
        void assertAttribute(const std::string& name);
#endif
    };

    // template methods
    template <typename T>
    void ShaderProgram::addUniforms() {
#ifdef DEBUG
            static_assert(std::is_base_of<ShaderUpdator, T>::value, 
                "T must be a subclass of ShaderUpdator");
#endif
        T::declareShaderUniforms(this);
    }


} // namespace mgl

#endif /* MGL_SHADERS_HPP */