#pragma once

namespace mgl {

    class ShaderBuilder;
    class ShaderProgram;

    class ShaderUpdator {
    public:
        /** 
         * @brief Called when creating a shader program to declare shader uniform's names
         */
        static void declareShaderUniforms(ShaderBuilder& shaders);

        /**
         * @brief Called at runtime to set the values on the predeclared shader uniform names
         */
        virtual void updateShaders(ShaderProgram& shaders) = 0;
    };
}