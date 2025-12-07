#pragma once

#include <memory>
#include "mgl/models/textures/mglSampler.hpp"
#include "mgl/models/textures/mglTexture.hpp"

namespace mgl {

/**
 * @brief Structure that holds information about a texture and its sampler,
 *        as well as the corresponding uniform in the shader program.
 * Should be used to bind textures and samplers to shader programs.
 * Some use cases:
 *   - Assign to materials to define their textures and samplers.
 *   - Change texture mid-program by updating the texture pointer
 *   - Change sampler parameters mid-program by updating the sampler object
 */
struct TextureSampler {
  GLenum unit;                // GL_TEXTUREi
  GLint index;               // sampler index in shader
  std::string uniform;        // uniform name in shader
  std::shared_ptr<Texture> texture = nullptr; // Texture (engine object)
  std::shared_ptr<Sampler> sampler = nullptr; // Sampler (engine object)

  TextureSampler(GLenum textureunit, GLint index, const std::string &uniform,
              std::shared_ptr<Texture> texture, std::shared_ptr<Sampler> sampler);
  void updateShader(ShaderProgram& shader);
};

}