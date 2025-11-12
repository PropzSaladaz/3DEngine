#pragma once

#include "math/vector.hpp"
#include "math/matrix.hpp"
#include "types.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <vector>
#include <string>
#include <map>

namespace mgl {

    /**
     * @brief Represents an already-compiled shader program consisting of multiple shaders (vertex, fragment, etc).
     * Serves as a handle to bind the shader program in OpenGL, as well as setting shader attributes and uniforms.
     * 
     */
    class ShaderProgram {
    public:

        // Stores the OpenGL's shader program object ID
        ui32 ProgramId;

        struct AttributeInfo { std::string name; i32 location; GLenum type; i32 size; };
        struct UniformInfo   { std::string name; i32 location; GLenum type; i32 size; };
        struct UboInfo       { std::string name; ui32 index; ui32 binding;  i32 dataSize; };

        std::unordered_map<std::string, AttributeInfo> Attributes;
        std::unordered_map<std::string, UniformInfo>   Uniforms;
        std::unordered_map<std::string, UboInfo>       Ubos;

        /**
         * @brief Creates a shader program and assigns it an ID returned from calling 'glCreateProgram'
         */
        ShaderProgram(ui32 programId, 
                      const std::unordered_map<std::string, AttributeInfo>& attributes = {},
                      const std::unordered_map<std::string, UniformInfo>& uniforms = {},
                      const std::unordered_map<std::string, UboInfo>& ubos = {}) 
            : ProgramId(programId), Attributes(attributes), Uniforms(uniforms), Ubos(ubos) 
        {};

        ~ShaderProgram();

        // =========== Query on attributes and uniforms =========== //

        bool isAttribute(const std::string &name);
        bool isUniform(const std::string &name);
        bool isUniformBlock(const std::string &name);


        void bind();
        void unbind();

        // =========== Uniform setters =========== //

        template <class T>
        void setUniform(const std::string& name, const T& value) {
            assertUniform(name);
            setUniform(Uniforms.at(name).location, value); // forwards to overloads below
        }
   

     private:
        // Basic types
        void setUniform(i32 loc, i32 v);
        void setUniform(i32 loc, bool v);
        void setUniform(i32 loc, f32 v);
        void setUniform(i32 loc, u32 v);

        // Vectors / Matrices
        void setUniform(i32 loc, const math::vec3& v);
        void setUniform(i32 loc, const math::vec4& v);
        void setUniform(i32 loc, const math::mat4& m);

        void assertUniform(const std::string &name);

    };
} // namespace mgl
