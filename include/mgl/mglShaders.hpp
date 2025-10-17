#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <map>

namespace mgl {

    class ShaderUpdator {
    public:
        /** 
         * @brief Called when creating a shader program to declare shader uniform's names
         * 
         */
        static void declareShaderUniforms(ShaderProgram* shaders);

        /**
         * @brief Called at runtime to set the values on the predeclared shader uniform names
         */
        virtual void updateShaders(ShaderProgram* shaders) = 0;
    };

    /**
     * @brief A shader program object is the final linked version of multiple shaders combined. 
     * To use any shader we must link them into a shader program object and then activate this shader program.
     */
    class ShaderProgram {
    public:

        // Stores the OpenGL's shader program object ID
        GLuint ProgramId;

        struct ShaderInfo {
            GLuint index;
        };

        // Links shader type to the OpenGL's shader object ID 
        // E.g:
        //      GL_VERTEX_SHADER -> 1234
        //      GL_FRAGMENT_SHADER -> 6173, and so on
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

        /**
         * @brief Creates a shader program and assigns it an ID returned from calling 'glCreateProgram'
         */
        ShaderProgram();

        ~ShaderProgram();

        /**
         * @brief Compiles the shader code from a source file and attaches it to the shader program.
         * @param shader_type specifies the type of shader to be added to
         * the program - GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, etc
         * @param filename file name where the shader is located
         */
        void addShader(const GLenum shader_type, const std::string &filename);

        void addAttribute(const std::string &name, const GLuint index);
        bool isAttribute(const std::string &name);
    
        template <typename T> void addUniforms();
        void addUniform(const std::string &name);
        bool isUniform(const std::string &name);
        void addUniformBlock(const std::string &name, const GLuint binding_point);
        bool isUniformBlock(const std::string &name);

        /**
         * @brief Links all added shaders to create the final shader program.
         */
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
        /**
         * @brief Checks whether a shader program has been compiled successfully or not.
         * If didn't compile successfully, exits the program
         * @param shader_id id of the compiled shader
         * @param filename shader's file name
         * @returns True if compiled successfully. False otherwise
         */
        const void checkCompilation(const GLuint shader_id,const std::string &filename);

        /** 
         * @brief Checks whether the shader program was linked correctly. If not, exits the program
         */
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
