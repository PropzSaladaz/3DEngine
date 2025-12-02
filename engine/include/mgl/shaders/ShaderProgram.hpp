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
        ui32 _programId;

        struct AttributeInfo { i32 location; };
        struct UniformInfo   { i32 location; };
        struct UboInfo       { i32 location; i32 bindingPoint; };

        std::unordered_map<std::string, AttributeInfo> _attributes;
        std::unordered_map<std::string, UniformInfo>   _uniforms;
        std::unordered_map<std::string, UboInfo>       _ubos;

        /**
         * @brief Creates a shader program and assigns it an ID returned from calling 'glCreateProgram'
         */
        ShaderProgram(ui32 programId,
                    std::unordered_map<std::string, AttributeInfo> attributes,
                    std::unordered_map<std::string, UniformInfo> uniforms,
                    std::unordered_map<std::string, UboInfo> ubos) noexcept
            : _programId(programId),
            _attributes(std::move(attributes)),
            _uniforms(std::move(uniforms)),
            _ubos(std::move(ubos)) {}

        ~ShaderProgram();

        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;

        // Move ctor
        ShaderProgram(ShaderProgram&& other) noexcept
            : _programId(other._programId),
            _attributes(std::move(other._attributes)),
            _uniforms(std::move(other._uniforms)),
            _ubos(std::move(other._ubos)) {
            other._programId = 0;  // don't let the moved-from object delete the program
        }

        // Move assign
        ShaderProgram& operator=(ShaderProgram&& other) noexcept {
            if (this != &other) {
                // Clean up current program
                if (_programId != 0) {
                    glDeleteProgram(_programId);
                }

                _programId = other._programId;
                _attributes = std::move(other._attributes);
                _uniforms  = std::move(other._uniforms);
                _ubos      = std::move(other._ubos);

                other._programId = 0;
            }
            return *this;
        }

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
            setUniform(_uniforms.at(name).location, value); // forwards to overloads below
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
