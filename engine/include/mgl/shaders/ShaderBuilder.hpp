#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <mgl/shaders/ShaderProgram.hpp>
#include <vector>
#include <string>
#include <map>
#include "math/math.hpp"

namespace mgl {

    class ShaderUpdator;

    /**
     * @brief Represents a shader program consisting of multiple shaders (vertex, fragment, etc).
     * Serves as a declarative interface to create, compile, link and use shader programs in OpenGL, 
     * as well as declaring and setting shader attributes and uniforms.
     */
    class ShaderBuilder {
    public:

        struct ShaderDecl { std::string fileName; };

        /**
         * @brief Holds info about a declared attribute in the shader program
         */
        struct AttributeDecl { i32 index; };

        /**
         * @brief Holds info about a declared uniform in the shader program
         */
        struct UniformDecl { i32 index; };

        /**
         * @brief Holds info about a declared UBO in the shader program
         */
        struct UboDecl { i32 index; ui32 binding_point; };

        // Links shader type to the OpenGL's shader object ID 
        // E.g:
        //      GL_VERTEX_SHADER -> 1234
        //      GL_FRAGMENT_SHADER -> 6173, and so on
        // std::unordered_map<GLenum, ShaderDecl> Shaders;

        /// Maps of attribute/uniform/ubo names to their declarations
        std::unordered_map<GLenum, ShaderDecl> Shaders;
        std::unordered_map<std::string, AttributeDecl> Attributes;
        std::unordered_map<std::string, UniformDecl> Uniforms;
        std::unordered_map<std::string, UboDecl> Ubos;

        ShaderBuilder();
        ~ShaderBuilder();

        /**
         * @brief Compiles the shader code from a source file and attaches it to the shader program.
         * @param shader_type specifies the type of shader to be added to
         * the program - GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, etc
         * @param filename file name where the shader is located
         */
        void addShader(const GLenum shader_type, const std::string &filename);

        // TODO - currently we use strings to identify attributes/uniforms.
        // In the future, it may be better to use enums, or just ints for performance
        void addAttribute(const std::string &name, const i32 index);
        bool isAttribute(const std::string &name);
    
        template <typename T> void addUniforms();
        void addUniform(const std::string &name);
        bool isUniform(const std::string &name);
        void addUniformBlock(const std::string &name, const ui32 binding_point);
        bool isUniformBlock(const std::string &name);

        /**
         * @brief Links all added shaders to create the final shader program.
         */
        ShaderProgram build();

     private:
        ui32 buildShader(const GLenum shader_type, const std::string &filename);
        /**
         * @brief Checks whether a shader program has been compiled successfully or not.
         * If didn't compile successfully, exits the program
         * @param shader_id id of the compiled shader
         * @param filename shader's file name
         * @returns True if compiled successfully. False otherwise
         */
        const void checkCompilation(const ui32 shader_id, const std::string &filename);

        /** 
         * @brief Checks whether the shader program was linked correctly. If not, exits the program
         */
        void checkLinkage(ui32 programId);
    };

    // template methods
    template <typename T>
    void ShaderBuilder::addUniforms() {
        static_assert(std::is_base_of<ShaderUpdator, T>::value, 
            "T must be a subclass of ShaderUpdator");
        T::declareShaderUniforms(this);
    }

} // namespace mgl
